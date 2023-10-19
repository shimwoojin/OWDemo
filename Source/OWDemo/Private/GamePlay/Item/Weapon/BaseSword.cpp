// If you want to use this project, mail to ggoggal@gmail.com

#include "BaseSword.h"
#include "MontagePlayDataAsset.h"
#include "BaseCharacter.h"
#include "Components/CapsuleComponent.h"

ABaseSword::ABaseSword()
{
#pragma region AssetSetup
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SWORD
	(TEXT("SkeletalMesh'/Game/Assets/Weapons/Weapon_Pack/Skeletal_Mesh/SK_Sword.SK_Sword'"));
	if (SWORD.Succeeded())
	{
		GetSkeletalMesh()->SetSkeletalMesh(SWORD.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> DRAW_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_TwoHandSwordDrawMontage.DA_TwoHandSwordDrawMontage'"));
	if (DRAW_MONTAGE.Succeeded())
	{
		DrawMontage = DRAW_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> SHEATH_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_TwoHandSwordSheathMontage.DA_TwoHandSwordSheathMontage'"));
	if (SHEATH_MONTAGE.Succeeded())
	{
		SheathMontage = SHEATH_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> ATTACK_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_TwoHandSwordAttackMontage.DA_TwoHandSwordAttackMontage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> ATTACK_ASSIST_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_TwoHandSwordAttackAssistMontage.DA_TwoHandSwordAttackAssistMontage'"));
	if (ATTACK_ASSIST_MONTAGE.Succeeded())
	{
		AttackAssistMontage = ATTACK_ASSIST_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> DODGE_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_TwoHandSwordDodgeMontage.DA_TwoHandSwordDodgeMontage'"));
	if (DODGE_MONTAGE.Succeeded())
	{
		DodgeMontage = DODGE_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> SWORD_TEXTURE
	(TEXT("Texture2D'/Game/Assets/Textures/Weapon/Sword.Sword'"));
	if (SWORD_TEXTURE.Succeeded())
	{
		ItemTexture = SWORD_TEXTURE.Object;
	}
#pragma endregion

	WeaponMode = EWeaponMode::Weapon;
	NormalWeaponType = ENormalWeaponType::TwoHandSword;
	EquipSocketName = "TwoHandSword_Equip";
	UnEquipSocketName = "TwoHandSword_UnEquip";
	bHandIK = true;
	ComboAttackDelay = .25f;

	DodgeDistance = 250.f;
	OnDodgeLineTraceDistance = 300.f;

	GuardEfficiency = .3f;

	SetItemName("Sword");
}

void ABaseSword::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor == GetOwner() || Cast<ABasePlayer>(OtherActor))
	{
		return;
	}

	if (!OneTimeDamagedActors.Contains(OtherActor) && IsValid(OtherActor))
	{
		FDamageEvent DamageEvent;
		OtherActor->TakeDamage(GetDamage(), DamageEvent, GetInstigatorController(), this);
		OneTimeDamagedActors.Add(OtherActor);
	}
}

void ABaseSword::Dash(bool bPressed)
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

void ABaseSword::OnDashNotify()
{
	if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		float CapsuleHalfHeight = BaseCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		float CapsuleRadius = BaseCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
		FVector LastInputVector = BaseCharacter->GetLastMovementInputVector();
		LastInputVector.Normalize();

		float Distance = DodgeDistance;
		FVector LineTraceStart = BaseCharacter->GetActorLocation() + LastInputVector * CapsuleRadius;
		FVector LineTraceEnd = LineTraceStart + LastInputVector * Distance;
		FHitResult HitResult;

		GetWorld()->LineTraceSingleByChannel(HitResult, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_Visibility);
		
		if (HitResult.bBlockingHit)
		{
			LineTraceStart = HitResult.Location - LastInputVector * CapsuleRadius + FVector(0.f, 0.f, CapsuleHalfHeight);
		}
		else
		{
			LineTraceStart = BaseCharacter->GetActorLocation() + LastInputVector * Distance + FVector(0.f, 0.f, CapsuleHalfHeight);
		}

		LineTraceEnd = LineTraceStart + FVector(0.f, 0.f, -OnDodgeLineTraceDistance);

		GetWorld()->LineTraceSingleByChannel(HitResult, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_Visibility);
		
		if (HitResult.bBlockingHit && !HitResult.bStartPenetrating)
		{
			FVector TeleportSpot = HitResult.ImpactPoint + FVector(0.f, 0.f, CapsuleHalfHeight);
			BaseCharacter->TeleportTo(TeleportSpot, BaseCharacter->GetActorRotation());
		}
	}
}

void ABaseSword::Attack(bool bPressed)
{
	if(auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		if (bPressed)
		{
			if (BaseCharacter->IsSameGaitState(EGaitState::Sit))
			{
				bLowAttack = true;
			}
			else if (BaseCharacter->IsSameGaitState(EGaitState::Jump))
			{
				bJumpAttack = true;
			}
			else
			{
				bNormalAttack = true;
				ReserveComboAttack();	//Change bNormalAttack With Timer
			}
		}
		else
		{
			if (bNormalAttack)
			{
				DoNormalAttack();
			}
			else if (bJumpAttack)
			{
				DoJumpAttack();
			}
			else if (bLowAttack)
			{
				DoLowAttack();
			}

			CancelComboAttack();
		}
	}
}

void ABaseSword::AttackAssist(bool bPressed)
{
	if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		if (bPressed)
		{
			GuardStart();
		}
		else
		{
			GuardEnd();
		}
	}
}

void ABaseSword::StartAttack()
{
	GetCapsule()->SetGenerateOverlapEvents(true);
}

void ABaseSword::EndAttack()
{
	GetCapsule()->SetGenerateOverlapEvents(false);
	OneTimeDamagedActors.Empty();
}

float ABaseSword::GuardDamageAsPercent()
{
	if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		UAnimInstance* CharacterAnimInstance = BaseCharacter->GetMesh()->GetAnimInstance();
		SMOOTH_CHECK(CharacterAnimInstance, 1.f);

		if (CharacterAnimInstance->Montage_IsPlaying(GuardStartMontage.AnimMontage))
		{
			FName CurrentSection = CharacterAnimInstance->Montage_GetCurrentSection();
			FName GuardSuccessSectionName = "Loop";

			if (CurrentSection == GuardSuccessSectionName)
			{
				if (SubWeaponExist())
				{
					return (1 - GetSubWeapon()->GetGuardEfficiency());
				}
				else
				{
					return (1 - GetGuardEfficiency());
				}
			}
		}
	}

	return 1.f;
}

void ABaseSword::ReserveComboAttack()
{
	GetWorld()->GetTimerManager().SetTimer(ComboAttackTimer, FTimerDelegate::CreateLambda([this]() {

		FMontagePlayData ComboAttackMontage;
		
		if (SubWeaponExist())
		{
			ComboAttackMontage = GetSubWeapon()->GetAttackMontage()->FindMontagePlayData("Combo");
		}
		else
		{
			ComboAttackMontage = AttackMontage->FindMontagePlayData("Combo");
		}

		if (ComboAttackMontage.PlayRate != 0.f)
		{
			if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
			{
				bool bIsAnyMontagePlaying = BaseCharacter->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();

				if (BaseCharacter->IsAttackable() && !bIsAnyMontagePlaying)
				{
					BaseCharacter->MontagePlay(ComboAttackMontage);
					BaseCharacter->SetState(ECharacterState::Attack);

					FOnMontageEnded OnMontageEnded;
					OnMontageEnded.BindLambda([BaseCharacter](UAnimMontage* AnimMontage, bool bInterrupted) {

						if (BaseCharacter->IsState(ECharacterState::Attack))
						{
							BaseCharacter->SetState(ECharacterState::Idle);
						}

						});

					BaseCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded, ComboAttackMontage.AnimMontage);

					bComboOn = true;
					bNormalAttack = false;
				}
			}
		}

		}), ComboAttackDelay, false);
}

void ABaseSword::CancelComboAttack()
{
	GetWorld()->GetTimerManager().ClearTimer(ComboAttackTimer);

	if (!bComboOn) return;

	if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		if (BaseCharacter->IsState(ECharacterState::Attack))
		{
			BaseCharacter->StopAnimMontage();
		}
	}

	bComboOn = false;
}

void ABaseSword::DoNormalAttack()
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

			FOnMontageEnded OnMontageEnded;
			OnMontageEnded.BindLambda([BaseCharacter](UAnimMontage* AnimMontage, bool bInterrupted) {

				if (!bInterrupted)
				{
					BaseCharacter->SetState(ECharacterState::Idle);
				}

				});

			BaseCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded, NormalAttackMontage.AnimMontage);

			bNormalAttack = false;
		}
	}
}

void ABaseSword::DoLowAttack()
{
	if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		FMontagePlayData LowAttackMontage;

		if (SubWeaponExist())
		{
			LowAttackMontage = GetSubWeapon()->GetAttackMontage()->FindMontagePlayData("Low");
		}
		else
		{
			LowAttackMontage = AttackMontage->FindMontagePlayData("Low");
		}

		bool bIsAnyMontagePlaying = BaseCharacter->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();

		if (LowAttackMontage.PlayRate != 0.f && BaseCharacter->IsAttackable() && !bIsAnyMontagePlaying)
		{
			BaseCharacter->MontagePlay(LowAttackMontage);
			BaseCharacter->SetState(ECharacterState::Attack);

			FOnMontageEnded OnMontageEnded;
			OnMontageEnded.BindLambda([BaseCharacter](UAnimMontage* AnimMontage, bool bInterrupted){

				if (!bInterrupted)
				{
					BaseCharacter->SetState(ECharacterState::Idle);
				}

				});

			BaseCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded, LowAttackMontage.AnimMontage);

			bLowAttack = false;
		}
	}
}

void ABaseSword::DoJumpAttack()
{
	if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		FMontagePlayData JumpAttackMontage;

		if (SubWeaponExist())
		{
			JumpAttackMontage = GetSubWeapon()->GetAttackMontage()->FindMontagePlayData("Jump");
		}
		else
		{
			JumpAttackMontage = AttackMontage->FindMontagePlayData("Jump");
		}

		bool bIsAnyMontagePlaying = BaseCharacter->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();

		if (JumpAttackMontage.PlayRate != 0.f && BaseCharacter->IsAttackable() && !bIsAnyMontagePlaying)
		{
			BaseCharacter->MontagePlay(JumpAttackMontage);
			BaseCharacter->SetState(ECharacterState::Attack);

			FOnMontageEnded OnMontageEnded;
			OnMontageEnded.BindLambda([BaseCharacter](UAnimMontage* AnimMontage, bool bInterrupted) {

				if (!bInterrupted)
				{
					BaseCharacter->SetState(ECharacterState::Idle);
				}

				});

			BaseCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded, JumpAttackMontage.AnimMontage);

			bJumpAttack = false;
		}
	}
}

void ABaseSword::GuardStart()
{
	if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		if (SubWeaponExist())
		{
			GuardStartMontage = GetSubWeapon()->GetAttackAssistMontage()->FindMontagePlayData("GuardStart");
		}
		else
		{
			GuardStartMontage = AttackAssistMontage->FindMontagePlayData("GuardStart");
		}

		if (GuardStartMontage.AnimMontage != nullptr && BaseCharacter->IsAttackAssistable())
		{
			BaseCharacter->MontagePlay(GuardStartMontage);
			BaseCharacter->SetState(ECharacterState::AttackAssist);
		}
	}
}

void ABaseSword::GuardEnd()
{
	if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		if (GuardStartMontage.AnimMontage)
		{
			float StartPosition = BaseCharacter->GetMesh()->GetAnimInstance()->Montage_GetPosition(GuardStartMontage.AnimMontage);
			FMontagePlayData GuardEndMontage;

			if (SubWeaponExist())
			{
				GuardEndMontage = GetSubWeapon()->GetAttackAssistMontage()->FindMontagePlayData("GuardEnd");
			}
			else
			{
				GuardEndMontage = AttackAssistMontage->FindMontagePlayData("GuardEnd");
			}

			GuardEndMontage.StartAt = StartPosition;

			if (GuardEndMontage.AnimMontage != nullptr && BaseCharacter->IsState(ECharacterState::AttackAssist))
			{
				BaseCharacter->MontagePlay(GuardEndMontage);

				FOnMontageEnded OnMontageEnded;
				OnMontageEnded.BindLambda([BaseCharacter](UAnimMontage* AnimMontage, bool bInterrupted) {

					if (!bInterrupted)
					{
						BaseCharacter->SetState(ECharacterState::Idle);
					}

					});

				BaseCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded, GuardEndMontage.AnimMontage);
			}
		}
	}
}