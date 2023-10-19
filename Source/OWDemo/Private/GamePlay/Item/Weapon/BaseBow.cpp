// If you want to use this project, mail to ggoggal@gmail.com

#include "BaseBow.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/TimelineComponent.h"
#include "MontagePlayDataAsset.h"
#include "BaseCharacter.h"
#include "Arrow.h"
#include "BasePlayer.h"
#include "BowAnim.h"

ABaseBow::ABaseBow()
{
	CreateDefaultSubobjectAuto(ChargeTimeline);
	CreateDefaultSubobjectAuto(ShotGuider);
	if (ShotGuider)
	{
		ShotGuider->SetupAttachment(GetRootComponent());
		ShotGuider->SetRelativeScale3D(FVector(.1f, .1f, .1f));
	}

#pragma region AssetSetup
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ARROW
	(TEXT("SkeletalMesh'/Game/Assets/Weapons/CityofBrass_Enemies/Meshes/Enemy/Archer/Archer_Bow_Weapon.Archer_Bow_Weapon'"));
	if (ARROW.Succeeded())
	{
		GetSkeletalMesh()->SetSkeletalMesh(ARROW.Object);
		//GetSkeletalMesh()->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));
		//GetCapsule()->SetRelativeScale3D(FVector(.2f, .2f, .2f));
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> BOW_ANIMCLASS
	(TEXT("AnimBlueprint'/Game/Blueprints/Actors/Weapon/ABP_Arrow.ABP_Arrow_C'"));
	if (BOW_ANIMCLASS.Succeeded())
	{
		GetSkeletalMesh()->SetAnimInstanceClass(BOW_ANIMCLASS.Class);
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> DRAW_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_BowDrawMontage.DA_BowDrawMontage'"));
	if (DRAW_MONTAGE.Succeeded())
	{
		DrawMontage = DRAW_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> SHEATH_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_BowSheathMontage.DA_BowSheathMontage'"));
	if (SHEATH_MONTAGE.Succeeded())
	{
		SheathMontage = SHEATH_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> ATTACK_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_BowAttackMontage.DA_BowAttackMontage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> DODGE_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_BowDodgeMontage.DA_BowDodgeMontage'"));
	if (DODGE_MONTAGE.Succeeded())
	{
		DodgeMontage = DODGE_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CHARGE_CURVE
	(TEXT("CurveFloat'/Game/Datas/Curve/BasicLinearFloat.BasicLinearFloat'"));
	if (CHARGE_CURVE.Succeeded())
	{
		ChargeCurve = CHARGE_CURVE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> BOW_TEXTURE
	(TEXT("Texture2D'/Game/Assets/Textures/Weapon/Bow.Bow'"));
	if (BOW_TEXTURE.Succeeded())
	{
		ItemTexture = BOW_TEXTURE.Object;
	}
#pragma endregion

	WeaponMode = EWeaponMode::Weapon;
	NormalWeaponType = ENormalWeaponType::Arrow;
	EquipSocketName = "Bow_Equip";
	UnEquipSocketName = "Bow_UnEquip";
	bHandIK = false;

	ArrowClass = AArrow::StaticClass();
	MaxArrowSpeed = 8000.f;
	MaxZoomTime = 1.5f;

	SetDamage(30.f);
	SetItemName("Bow");
}

void ABaseBow::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FOnTimelineFloat ChargeDelegate;
	ChargeDelegate.BindDynamic(this, &ThisClass::ChargeArrowDistance);

	ChargeTimeline->AddInterpFloat(ChargeCurve, ChargeDelegate);
	ChargeTimeline->SetPlayRate(1 / MaxZoomTime);
}

void ABaseBow::Dash(bool bPressed)
{
	if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		if (bPressed)
		{
			checkNoEntry();
		}
		else
		{
			if (BaseCharacter->IsDashable())
			{
				FMontagePlayData DodgeMontageData;

				if (SubWeaponExist())
				{
					DodgeMontageData = GetSubWeapon()->GetDodgeMontage()->GetMontagePlayData(0);
				}
				else
				{
					DodgeMontageData = DodgeMontage->GetMontagePlayData(0);
				}

				if (DodgeMontageData.AnimMontage)
				{
					FOnMontageEnded OnMontageEnded;
					OnMontageEnded.BindLambda([this, BaseCharacter](UAnimMontage* AnimMontage, bool bInterrupted) {

						if (!bInterrupted)
						{
							BaseCharacter->SetState(ECharacterState::Idle);
						}

						BaseCharacter->SetCanBeDamaged(true);

						});

					BaseCharacter->MontagePlay(DodgeMontageData);
					BaseCharacter->SetState(ECharacterState::Dash);
					BaseCharacter->SetCanBeDamaged(false);
					BaseCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded, DodgeMontageData.AnimMontage);
				}
			}
		}
	}
}

void ABaseBow::Attack(bool bPressed)
{
	if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		if (bPressed)
		{
			DrawArrow();
		}
		else
		{
			ShotArrow();
		}
	}
}

void ABaseBow::AttackAssist(bool bPressed)
{
}

void ABaseBow::DrawArrow()
{
	if (SubWeaponExist())
	{
		DrawAndShotMontage = GetSubWeapon()->GetAttackMontage()->FindMontagePlayData("DrawAndShot");
	}
	else
	{
		DrawAndShotMontage = AttackMontage->FindMontagePlayData("DrawAndShot");
	}

	if (DrawAndShotMontage.PlayRate != 0.f)
	{
		if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
		{
			bool bIsAnyMontagePlaying = BaseCharacter->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();

			if (BaseCharacter->IsAttackable() && !bIsAnyMontagePlaying)
			{
				//MontagePlay, Montage EndDelegate Setup
				BaseCharacter->MontagePlay(DrawAndShotMontage);
				BaseCharacter->SetState(ECharacterState::Attack);

				FOnMontageEnded OnMontageEnded;
				OnMontageEnded.BindLambda([this, BaseCharacter](UAnimMontage* AnimMontage, bool bInterrupted) {

					if (!bInterrupted)
					{
						BaseCharacter->SetState(ECharacterState::Idle);
					}
					else
					{
						if (BeforeShotedArrow)
						{
							BeforeShotedArrow->Destroy();
						}

						if (auto* const BasePlayer = Cast<ABasePlayer>(GetOwner()))
						{
							BasePlayer->SetGaugePercent(0.f);
							BasePlayer->HideGauge();
						}
					}

					});
				BaseCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded, DrawAndShotMontage.AnimMontage);

				//ArrowSpawn, AttachToBow
				FActorSpawnParameters ArrowSpawnParameters;
				ArrowSpawnParameters.Instigator = GetInstigator();
				ArrowSpawnParameters.Owner = this;
				BeforeShotedArrow = GetWorld()->SpawnActor<AArrow>(ArrowClass, ArrowSpawnParameters);
				FAttachmentTransformRules AttachRule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
				BeforeShotedArrow->AttachToComponent(ShotGuider, AttachRule);

				ChargeTimeline->PlayFromStart();

				if (auto* const BasePlayer = Cast<ABasePlayer>(BaseCharacter))
				{
					BasePlayer->ShowGauge();
					BasePlayer->SetGaugePercent(0.f);
				}
			}
		}
	}

}

void ABaseBow::ShotArrow()
{
	if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		bool bChargeArrow = BaseCharacter->GetMesh()->GetAnimInstance()->Montage_IsPlaying(DrawAndShotMontage.AnimMontage);

		if (bChargeArrow)
		{
			BaseCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection("AttackStart");

			if (IsValid(BeforeShotedArrow))
			{
				BeforeShotedArrow->Initialize(ChargedArrowSpeed, 1.f, GetDamage(), BaseCharacter->GetControlRotation());
				FDetachmentTransformRules DetachRule(EDetachmentRule::KeepWorld, false);
				BeforeShotedArrow->DetachFromActor(DetachRule);
				BeforeShotedArrow = nullptr;
				ChargeTimeline->Stop();

				if (auto* const BasePlayer = Cast<ABasePlayer>(GetOwner()))
				{
					BasePlayer->SetGaugePercent(0.f);
					BasePlayer->HideGauge();
				}
			}
		}
	}
}

void ABaseBow::StartAttack()
{
	if (bSkeletalMesh)
	{
		auto* BowAnim = Cast<UBowAnim>(GetSkeletalMesh()->GetAnimInstance());
		auto* BaseCharacter = Cast<ABaseCharacter>(GetOwner());

		if (BowAnim && BaseCharacter)
		{
			FVector RightHandLocation = BaseCharacter->GetRightHandLocation();
			BowAnim->SetArrowLineLocation(RightHandLocation);
			BowAnim->SetArrowLineAlpha(1.0f);
		}
	}
}

void ABaseBow::AttackTick(float DeltaTime, float TotalDuration)
{
	if (bSkeletalMesh)
	{
		auto* BowAnim = Cast<UBowAnim>(GetSkeletalMesh()->GetAnimInstance());
		auto* BaseCharacter = Cast<ABaseCharacter>(GetOwner());

		if (BowAnim && BaseCharacter)
		{
			FVector RightHandLocation = BaseCharacter->GetRightHandLocation();
			BowAnim->SetArrowLineLocation(RightHandLocation);
		}
	}
}

void ABaseBow::EndAttack()
{
	if (bSkeletalMesh)
	{
		auto* BowAnim = Cast<UBowAnim>(GetSkeletalMesh()->GetAnimInstance());
		auto* BaseCharacter = Cast<ABaseCharacter>(GetOwner());

		if (BowAnim && BaseCharacter)
		{
			FVector RightHandLocation = BaseCharacter->GetRightHandLocation();
			//BowAnim->SetArrowLineLocation(RightHandLocation);
			BowAnim->SetArrowLineAlpha(.0f);
		}
	}
}

void ABaseBow::ChargeArrowDistance(float Alpha)
{
	ChargedArrowSpeed = 2000.f + MaxArrowSpeed * Alpha;

	if (auto* const BasePlayer = Cast<ABasePlayer>(GetOwner()))
	{
		BasePlayer->SetGaugePercent(Alpha);
	}
}
