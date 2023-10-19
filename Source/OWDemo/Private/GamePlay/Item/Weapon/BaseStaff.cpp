// If you want to use this project, mail to ggoggal@gmail.com

#include "BaseStaff.h"
#include "BaseCharacter.h"
#include "MontagePlayDataAsset.h"
#include "Components/TimelineComponent.h"
#include "ThunderBall.h"
#include "WaterBall.h"
#include "BasePlayer.h"
#include "StaffNormalSkill.h"

ABaseStaff::ABaseStaff()
{
	CreateDefaultSubobjectAuto(ChargeTimeline);

#pragma region AssetSetup
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> STAFF
	(TEXT("SkeletalMesh'/Game/Assets/Weapons/Staff/staff.staff'"));
	if (STAFF.Succeeded())
	{
		GetSkeletalMesh()->SetSkeletalMesh(STAFF.Object);
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CHARGE_CURVE
	(TEXT("CurveFloat'/Game/Datas/Curve/BasicLinearFloat.BasicLinearFloat'"));
	if (CHARGE_CURVE.Succeeded())
	{
		ChargeCurve = CHARGE_CURVE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> DRAW_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_StaffDrawMontage.DA_StaffDrawMontage'"));
	if (DRAW_MONTAGE.Succeeded())
	{
		DrawMontage = DRAW_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> SHEATH_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_StaffSheatMontage.DA_StaffSheatMontage'"));
	if (SHEATH_MONTAGE.Succeeded())
	{
		SheathMontage = SHEATH_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> ATTACK_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_StaffAttackMontage.DA_StaffAttackMontage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> DODGE_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_StaffDodgeMontage.DA_StaffDodgeMontage'"));
	if (DODGE_MONTAGE.Succeeded())
	{
		DodgeMontage = DODGE_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> STAFF_TEXTURE
	(TEXT("Texture2D'/Game/Assets/Textures/Weapon/Staff.Staff'"));
	if (STAFF_TEXTURE.Succeeded())
	{
		ItemTexture = STAFF_TEXTURE.Object;
	}
#pragma endregion

	WeaponMode = EWeaponMode::Weapon;
	NormalWeaponType = ENormalWeaponType::Staff;
	EquipSocketName = "Staff_Equip";
	UnEquipSocketName = "Staff_UnEquip";
	bHandIK = false;
	ChargeAttackDelay = .25f;

	ElementalBallProperty.MaxSpeed = 5000.f;
	ElementalBallProperty.DefaultSpeed = 3000.f;
	ElementalBallProperty.MaxSize = 2.5f;
	ElementalBallProperty.MaxChargeTime = 1.5f;

	ElementalBallClassMap.Add(EElementalType::Thunder, AThunderBall::StaticClass());
	ElementalBallClassMap.Add(EElementalType::Water, AWaterBall::StaticClass());

	NormalSkillClass = AStaffNormalSkill::StaticClass();

	SetDamage(30.f);
	SetItemName("Staff");
}

void ABaseStaff::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (ChargeCurve)
	{
		FOnTimelineFloat ChargeDelegate;
		ChargeDelegate.BindDynamic(this, &ThisClass::ChargeElementalBall);

		ChargeTimeline->AddInterpFloat(ChargeCurve, ChargeDelegate);
		ChargeTimeline->SetPlayRate(1 / ElementalBallProperty.MaxChargeTime);
	}
	else
	{
		LOG(Warning, TEXT("No ChargeCurve!"));
	}
}

void ABaseStaff::Dash(bool bPressed)
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

void ABaseStaff::Attack(bool bPressed)
{
	if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		if (bPressed)
		{
			if (!BaseCharacter->IsSameGaitState(EGaitState::Jump) && !BaseCharacter->IsSameGaitState(EGaitState::Sit))
			{
				SelectedElementalBallClass = (UClass*)(*ElementalBallClassMap.Find(ElementalTypeNow));

				bNormalAttack = true;
				ReserveChargeAttack();
			}
		}
		else
		{
			if (bChargeAttack)
			{
				BaseCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection("AttackStart", ChargeAttackMontage.AnimMontage);
				ChargeTimeline->Stop();
			}
			else
			{
				GetWorldTimerManager().ClearTimer(ChargeAttackHandle);

				if (bNormalAttack)
				{
					DoNormalAttack();
				}
			}
		}
	}
}

void ABaseStaff::AttackAssist(bool bPressed)
{
}

void ABaseStaff::ChangeElementalType()
{
	ElementalTypeNow = static_cast<EElementalType>((((int32)ElementalTypeNow + 1) % (int32)EElementalType::Max));
}

void ABaseStaff::ReserveChargeAttack()
{
	GetWorldTimerManager().SetTimer(ChargeAttackHandle, FTimerDelegate::CreateLambda([this]() {
		
		if (SubWeaponExist())
		{
			ChargeAttackMontage = GetSubWeapon()->GetAttackMontage()->FindMontagePlayData("Charge");
		}
		else
		{
			ChargeAttackMontage = AttackMontage->FindMontagePlayData("Charge");
		}

		if (ChargeAttackMontage.PlayRate != 0.f)
		{
			if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
			{
				bool bIsAnyMontagePlaying = BaseCharacter->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();

				if (BaseCharacter->IsAttackable() && !bIsAnyMontagePlaying)
				{
					BaseCharacter->MontagePlay(ChargeAttackMontage);
					BaseCharacter->SetState(ECharacterState::Attack);
					SpawnElementalBall();
					ChargeTimeline->PlayFromStart();

					if (auto* const BasePlayer = Cast<ABasePlayer>(BaseCharacter))
					{
						BasePlayer->ShowGauge();
						BasePlayer->SetGaugePercent(0.f);
					}

					FOnMontageEnded OnMontageEnded;
					OnMontageEnded.BindLambda([this, BaseCharacter](UAnimMontage* AnimMontage, bool bInterrupted) {

						if (!bInterrupted)
						{
							BaseCharacter->SetState(ECharacterState::Idle);
						}
						else
						{
							if (ElementalBall)
							{
								ElementalBall->Destroy();

							}

							if (auto* const BasePlayer = Cast<ABasePlayer>(GetOwner()))
							{
								BasePlayer->SetGaugePercent(0.f);
								BasePlayer->HideGauge();
							}
						}

						bChargeAttack = false;

						});

					BaseCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded, ChargeAttackMontage.AnimMontage);

					bChargeAttack = true;
					bNormalAttack = false;
				}
			}
		}

		}), ChargeAttackDelay, false);
}

void ABaseStaff::DoNormalAttack()
{
	if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		FMontagePlayData NormalAttackMontage;

		if (SubWeaponExist())
		{
			NormalAttackMontage = GetSubWeapon()->GetAttackMontage()->FindMontagePlayData("Normal");
		}
		else
		{
			NormalAttackMontage = AttackMontage->FindMontagePlayData("Normal");
		}

		bool bIsAnyMontagePlaying = BaseCharacter->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();

		if (NormalAttackMontage.PlayRate != 0.f && BaseCharacter->IsAttackable() && !bIsAnyMontagePlaying)
		{
			BaseCharacter->MontagePlay(NormalAttackMontage);
			BaseCharacter->SetState(ECharacterState::Attack);
			SpawnElementalBall();

			FOnMontageEnded OnMontageEnded;
			OnMontageEnded.BindLambda([this, BaseCharacter](UAnimMontage* AnimMontage, bool bInterrupted) {

				if (!bInterrupted)
				{
					BaseCharacter->SetState(ECharacterState::Idle);
				}
				else
				{
					if (ElementalBall)
					{
						ElementalBall->Destroy();
					}
				}

				});

			BaseCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded, NormalAttackMontage.AnimMontage);

			bNormalAttack = false;
			ElementalBallProperty.Speed = ElementalBallProperty.DefaultSpeed;
		}
	}
}

void ABaseStaff::StartAttack()
{

}

void ABaseStaff::EndAttack()
{
	if (ElementalBall)
	{
		FDetachmentTransformRules DetachRule(EDetachmentRule::KeepWorld, false);
		ElementalBall->DetachFromActor(DetachRule);
		ElementalBall->Initialize(ElementalBallProperty.Speed, GetDamage(), GetInstigator()->GetControlRotation());
		ElementalBall->EnableOverlapEvents();
		ElementalBall->SetLifeSpan(10.f);
		ElementalBall = nullptr;

		if (auto* const BasePlayer = Cast<ABasePlayer>(GetOwner()))
		{
			BasePlayer->SetGaugePercent(0.f);
			BasePlayer->HideGauge();
		}
	}
}

void ABaseStaff::SpawnElementalBall()
{
	FName AttachSocketName = "ElementalBall_Socket";

	FActorSpawnParameters ElementalBallSpawnParameters;
	ElementalBallSpawnParameters.Instigator = Cast<APawn>(GetOwner());
	ElementalBallSpawnParameters.Owner = this;

	FTransform ElementalBallSpawnTransform = GetTransform();
	ElementalBallSpawnTransform.SetLocation(GetSkeletalMesh()->GetSocketLocation(AttachSocketName));

	ElementalBall = GetWorld()->SpawnActor<AElementalBall>(SelectedElementalBallClass, ElementalBallSpawnTransform, ElementalBallSpawnParameters);
	if (ElementalBall)
	{
		FAttachmentTransformRules AttachRule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
		ElementalBall->AttachToComponent(GetSkeletalMesh(), AttachRule, AttachSocketName);
	}
}

void ABaseStaff::ChargeElementalBall(float Alpha)
{
	if (IsValid(ElementalBall))
	{
		float ElementalBallSize = FMath::Lerp(1.f, ElementalBallProperty.MaxSize, Alpha);
		FVector ElementalBallSizeVector = FVector::OneVector * ElementalBallSize;

		ElementalBall->SetActorScale3D(ElementalBallSizeVector);
		ElementalBallProperty.Speed = FMath::Lerp(ElementalBallProperty.DefaultSpeed, ElementalBallProperty.MaxSpeed, Alpha);

		if (auto* const BasePlayer = Cast<ABasePlayer>(GetOwner()))
		{
			BasePlayer->SetGaugePercent(Alpha);
		}
	}
}
