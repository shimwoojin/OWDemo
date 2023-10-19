// If you want to use this project, mail to ggoggal@gmail.com

#include "BaseGun.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "BasePlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "MontagePlayDataAsset.h"
#include "BulletFactory.h"
#include "Kismet/KismetMathLibrary.h"
#include "OWDemoSetting.h"
#include "PlayerAnimInstance.h"
#include "GunShotCameraShake.h"
#include "Blueprint/UserWidget.h"
#include "BasePlayerController.h"
#include "PlayerUI.h"

ABaseGun::ABaseGun()
{
	//Init Components
	CreateDefaultSubobjectAuto(MuzzleFlash);
	CreateDefaultSubobjectAuto(ShootingSound);
	MuzzleFlash->SetAutoActivate(false);
	ShootingSound->SetAutoActivate(false);

	//Init Properties
	WeaponMode = EWeaponMode::Soldier;
	bHandIK = true;
	MuzzleSocket = "Muzzle";
	AutoFireRate = .125f;
	SingleFireRate = .15f;
	bCanFire = true;
	MaxBullets = 10;
	EquipedBullets = MaxBullets;
	ZoomProperty.ZoomRate = 1.5f;

	GunShotValidDistance = 20000.f;
	BulletSpeed = 10000.f;

	ZoomTimeline = MakeShared<FTimeline>();
	GunShotCameraShakeClass = UGunShotCameraShake::StaticClass();
}

void ABaseGun::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (auto* const AttachMesh = GetSkeletalMesh())
	{
		FAttachmentTransformRules AttachRule(EAttachmentRule::SnapToTarget, false);

		MuzzleFlash->AttachToComponent(AttachMesh, AttachRule, MuzzleSocket);
		ShootingSound->AttachToComponent(AttachMesh, AttachRule, MuzzleSocket);
	}
}

void ABaseGun::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ZoomTimeline->TickTimeline(DeltaSeconds);
}

void ABaseGun::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseGun::WeaponInitialize(class ABaseCharacter* const Character)
{
	Super::WeaponInitialize(Character);

	if (auto* const BasePlayer = Cast<ABasePlayer>(GetOwner()))
	{
		//Zoom
		ZoomProperty.BaseFieldOfView = BasePlayer->GetCamera()->FieldOfView;

		if (ZoomCurve != nullptr)
		{
			FOnTimelineFloatStatic OnTimelineFloat;
			OnTimelineFloat.BindUObject(BasePlayer, &ABasePlayer::Zoom, ZoomProperty);
			ZoomTimeline->AddInterpFloat(ZoomCurve, OnTimelineFloat);
			ZoomTimeline->SetPlayRate(1 / ZoomProperty.ZoomSpeed);
		}

		if (ZoomInUIClass)
		{
			ZoomInUI = CreateWidget(Cast<APlayerController>(Character->GetController()), ZoomInUIClass);
		}

		if (ZoomInUI)
		{
			ZoomInUI->AddToViewport(1);
			ZoomInUI->SetVisibility(ESlateVisibility::Collapsed);
		}

		//MontageEnd
		UAnimInstance* const PlayerAnim = BasePlayer->GetMesh()->GetAnimInstance();
		PlayerAnim->OnMontageEnded.AddDynamic(this, &ThisClass::OnFireEnd);
	}
	else
	{
		LOG(Warning, TEXT("WeaponInitialize Is Not Played, Owner == nullptr || NotBasePlayer"));
	}
}

FString ABaseGun::GetItemInfo()
{
	FString GunTriggerInfo;

	switch (GunTrigger)
	{
	case EGunTriggerGroup::SingleFire:
		GunTriggerInfo = "Single";
		break;
	case EGunTriggerGroup::AutoFire:
		GunTriggerInfo = "Auto";
		break;
	}

	return GunTriggerInfo + " " + "[" + FString::FromInt(EquipedBullets) + " / " + FString::FromInt(MaxBullets) + "]";
}

void ABaseGun::Attack(bool bPressed)
{
	if (bPressed)
	{
		switch (GunTrigger)
		{
		case EGunTriggerGroup::SingleFire:
		{
			if (bCanFire)
			{
				Fire();
				bCanFire = false;

				FTimerHandle SingleFireHandle;
				GetWorldTimerManager().SetTimer(SingleFireHandle, FTimerDelegate::CreateLambda([this, &SingleFireHandle]() {
					
					bCanFire = true;
					GetWorldTimerManager().ClearTimer(SingleFireHandle);
					
					}) , SingleFireRate, false);
			}

			break;
		}
		case EGunTriggerGroup::AutoFire:
		{
			if (bCanFire)
			{
				GetWorldTimerManager().SetTimer(FireHandle, this, &ThisClass::Fire, AutoFireRate, true, 0);
			}
			break;
		}
		}
	}
	else
	{
		StopFire();
	}
}

void ABaseGun::AttackAssist(bool bPressed)
{
	if (ZoomProperty.bZoomable)
	{
		if (bPressed)
		{
			ZoomIn();
		}
		else
		{
			ZoomOut();
		}
	}
}

void ABaseGun::Fire()
{
	auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner());
	auto* const BasePlayerController = Cast<ABasePlayerController>(GetInstigatorController());

	if (BaseCharacter)
	{
		if (BaseCharacter->IsAttackable())
		{
			if (EquipedBullets > 0)
			{
				if (GunShotMontage)
				{
					FMontagePlayData GunShotMontageData = GunShotMontage->GetMontagePlayData(0);

					BaseCharacter->MontagePlay(GunShotMontageData);
					BaseCharacter->SetState(ECharacterState::Attack);

					if (auto* const ViewManage = Cast<IViewManagement>(BaseCharacter))
					{
						ViewManage->CameraShake(GunShotCameraShakeClass);
					}
					
					MuzzleFlash->Activate(true);
					ShootingSound->Play();
					SpawnBullet();

					--EquipedBullets;

					if (BasePlayerController)
					{
						UPlayerUI* PlayerUI = BasePlayerController->GetPlayerUI();
						if (PlayerUI)
						{
							PlayerUI->UpdateWeaponInfoText();
						}
					}
				}
			}

			if (EquipedBullets == 0)
			{
				Reload();
			}
		}
	}
}

void ABaseGun::StopFire()
{
	GetWorldTimerManager().ClearTimer(FireHandle);
}

void ABaseGun::OnFireEnd(class UAnimMontage* InAnimMontage, bool bInterrupted)
{
	FMontagePlayData GunShotMontageData = GunShotMontage->GetMontagePlayData(0);

	if ((GunShotMontageData.AnimMontage == InAnimMontage))
	{
		if (auto* BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
		{
			if (!bInterrupted)
			{
				BaseCharacter->SetState(ECharacterState::Idle);
			}
		}
	}
}

void ABaseGun::ZoomIn()
{
	ZoomTimeline->Play();
	if (ZoomInUI)
	{
		ZoomInUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void ABaseGun::ZoomOut()
{
	ZoomTimeline->Reverse();
	if (ZoomInUI)
	{
		ZoomInUI->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ABaseGun::Reload()
{
	auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner());

	if (BaseCharacter)
	{
		if (ReloadMontage && BaseCharacter->IsReloadable())
		{
			UAnimInstance* const PlayerAnim = BaseCharacter->GetMesh()->GetAnimInstance();

			if (PlayerAnim)
			{
				BaseCharacter->SetState(ECharacterState::Reload);
				BaseCharacter->MontagePlay(ReloadMontage->GetMontagePlayData(0));

				FMontagePlayData ReloadMontageData = ReloadMontage->GetMontagePlayData(0);
				FOnMontageEnded ReloadSuccess = FOnMontageEnded::CreateUObject(this, &ThisClass::ReloadSuccess);
			
				PlayerAnim->Montage_SetEndDelegate(ReloadSuccess, ReloadMontageData.AnimMontage);
			}
		}
	}
}

void ABaseGun::ChangeGunTrigger()
{
	GunTrigger = ChangeableGunTriggers[((int32)GunTrigger + 1) % ChangeableGunTriggers.Num()];

	auto* const BasePlayerController = Cast<ABasePlayerController>(GetInstigatorController());

	if (BasePlayerController)
	{
		UPlayerUI* PlayerUI = BasePlayerController->GetPlayerUI();
		if (PlayerUI)
		{
			PlayerUI->UpdateWeaponInfoText();
		}
	}
}

void ABaseGun::ReloadSuccess(UAnimMontage* InReloadMontage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		auto* const BasePlayerController = Cast<ABasePlayerController>(GetInstigatorController());

		EquipedBullets = MaxBullets;

		if (BasePlayerController)
		{
			UPlayerUI* PlayerUI = BasePlayerController->GetPlayerUI();
			if (PlayerUI)
			{
				PlayerUI->UpdateWeaponInfoText();
			}
		}
	}

	if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		if (BaseCharacter->IsState(ECharacterState::Reload))
		{
			BaseCharacter->SetState(ECharacterState::Idle);
		}
	}
}

void ABaseGun::SpawnBullet()
{
	auto* const BasePlayer = Cast<ABasePlayer>(GetOwner());

	if (BasePlayer)
	{
		FVector CameraLocation = BasePlayer->GetCamera()->GetComponentLocation();
		FVector CameraForward = BasePlayer->GetCamera()->GetForwardVector();
		FVector LineTraceEnd = CameraLocation + CameraForward * GunShotValidDistance;

		FHitResult HitResult;

		GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, LineTraceEnd, ECollisionChannel::ECC_Visibility);

		FVector StartLocation = MuzzleFlash->GetComponentLocation();;
		FVector EndLocation;
		FRotator BulletSpawnDirection;

		if (HitResult.bBlockingHit)
		{
			EndLocation = HitResult.Location;
		}
		else
		{
			EndLocation = StartLocation + CameraForward * GunShotValidDistance;
		}

		BulletSpawnDirection = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);

		ABullet* Bullet = NewObject<UBulletFactory>(this)
			->SetOwner(this)
			->SetInstigator(GetInstigator())
			->SetDamage(GetDamage())
			->SetSpeed(BulletSpeed)
			->SetStaticmesh(BulletMesh)
			->SetSpawnDirectionAndLocation(BulletSpawnDirection, StartLocation)
			->SetWeaponType(SoldierWeaponType)
			->BuildBullet();
	}
}
