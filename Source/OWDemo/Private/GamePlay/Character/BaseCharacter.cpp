//If you want to use this project, mail to ggoggal@gmail.com

#include "BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "StatComponent.h"
#include "StateComponent.h"
#include "CharacterAnimInstance.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MontagePlayDataAsset.h"
#include "BasePlayer.h"
#include "ModeManageComponent.h"
#include "WeaponComponent.h"
#include "BaseWeapon.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CreateDefaultSubobjectAuto(Stat);
	CreateDefaultSubobjectAuto(State);
	CreateDefaultSubobjectAuto(SprintTimeline);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	GetCapsuleComponent()->SetCollisionProfileName(OWCollision::PresetName_Character);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> SPRINT_CURVE
	(TEXT("CurveFloat'/Game/Datas/Curve/BasicLinearFloat.BasicLinearFloat'"));
	if (SPRINT_CURVE.Succeeded())
	{
		SprintCurve = SPRINT_CURVE.Object;
	}

	SprintConvertTime = .3f;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FOnTimelineFloat SprintDelegate;
	SprintDelegate.BindDynamic(this, &ThisClass::OnSprint);

	SprintTimeline->AddInterpFloat(SprintCurve, SprintDelegate);
	SprintTimeline->SetPlayRate(1 / SprintConvertTime);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	auto* CharacterAnim = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	if (CharacterAnim)
	{
		CharacterAnim->SetCrouched(true);
		State->SetGaitState(EGaitState::Sit);
	}
}

void ABaseCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	auto* CharacterAnim = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	if (CharacterAnim)
	{
		CharacterAnim->SetCrouched(false);
		State->SetGaitState(EGaitState::Stand);
	}
}

float ABaseCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageAfter = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (IsState(ECharacterState::Death))
	{
		return 0.f;
	}

	EHittedDirection HittedDirection = CalculateHitFromDirection(DamageCauser, EventInstigator);

	if (EHittedDirection::Forward == HittedDirection)
	{
		if (ABasePlayer* BasePlayer = Cast<ABasePlayer>(this))
		{
			if (UWeaponComponent* CurrentWeaponComponent = BasePlayer->GetModeManager()->GetCurrentWeaponComponent())
			{
				ABaseWeapon* ArmingWeapon = CurrentWeaponComponent->GetArmingWeapon();

				if (ArmingWeapon)
				{
					DamageAfter *= ArmingWeapon->GuardDamageAsPercent();
				}
			}
		}
	}
	
	float CurrentHP = AddCurrentHP(-DamageAfter);
	FMontagePlayData HitMontagePlayData;
	FMontagePlayData DeathMontagePlayData;

	if (CurrentHP <= 0.f)
	{
		SetState(ECharacterState::Death);

		DeathMontagePlayData = DeathMontage->GetMontagePlayData(0);

		if (DeathMontagePlayData.AnimMontage)
		{
			MontagePlay(DeathMontagePlayData);
			
			FOnMontageEnded OnMontageBlended;
			OnMontageBlended.BindLambda([this](UAnimMontage* AnimMontage, bool bInterrupted) {
				Destroy();
				});

			GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageBlended, DeathMontagePlayData.AnimMontage);
		}
		else
		{
			Destroy();
		}
	}

	if (IsHitMontagePlayable() && !IsState(ECharacterState::Hitted))
	{
		if (HittedMontage)
		{
			switch (HittedDirection)
			{
			case EHittedDirection::Any:
			{
				HitMontagePlayData = HittedMontage->GetMontagePlayData(0);
				break;
			}
			case EHittedDirection::Forward:
			{
				HitMontagePlayData = HittedMontage->FindMontagePlayData("Forward");
				break;
			}
			case EHittedDirection::Backward:
				HitMontagePlayData = HittedMontage->FindMontagePlayData("Backward");
				break;
			case EHittedDirection::Left:
				HitMontagePlayData = HittedMontage->FindMontagePlayData("Left");
				break;
			case EHittedDirection::Right:
				HitMontagePlayData = HittedMontage->FindMontagePlayData("Right");
				break;
			}
		}

		if (HitMontagePlayData.AnimMontage)
		{
			MontagePlay(HitMontagePlayData);
			SetState(ECharacterState::Hitted);

			FOnMontageEnded OnMontageEnded;
			OnMontageEnded.BindLambda([this](UAnimMontage* AnimMontage, bool bInterrupted) {
				
				if (!bInterrupted)
				{
					SetState(ECharacterState::Idle);
				}

				});
			
			GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded, HitMontagePlayData.AnimMontage);
		}
	}

	return DamageAfter;
}

void ABaseCharacter::MontagePlay(const FMontagePlayData& MontagePlayData)
{
	SMOOTH_CHECK(MontagePlayData.AnimMontage);

	GetMesh()->GetAnimInstance()->Montage_Play(
		MontagePlayData.AnimMontage,
		MontagePlayData.PlayRate,
		MontagePlayData.ReturnType,
		MontagePlayData.StartAt,
		MontagePlayData.StopAllMontages
	);
}

float ABaseCharacter::SetMaxHP(float MaxHP, bool bEffectToCurrentHP)
{
	SetStat(EStat::MaxHP, MaxHP);
	FStat StatMaxHP = GetStat(EStat::MaxHP);

	if (StatMaxHP.IsValidStat() && bEffectToCurrentHP)
	{
		SetStat(EStat::CurrentHP, StatMaxHP.Value);
	}

	return StatMaxHP.Value;
}

float ABaseCharacter::AddCurrentHP(float AddHP)
{
	Stat->AddStat(EStat::CurrentHP, AddHP);

	return Stat->GetStat(EStat::CurrentHP).Value;
}

void ABaseCharacter::PickUpItem_MontagePlay()
{
	if(IsState(ECharacterState::Idle) && PickUpMontage)
	{
		MontagePlay(PickUpMontage->GetMontagePlayData(0));
	}
}

FVector ABaseCharacter::GetRightHandLocation()
{
	return GetMesh()->GetBoneLocation("hand_r");
}

void ABaseCharacter::Dash(bool bPressed)
{

}

void ABaseCharacter::Sprint(bool bPressed)
{
	if (bPressed)
	{
		SprintTimeline->Play();
	}
	else
	{
		SprintTimeline->Reverse();
	}
}

void ABaseCharacter::Run()
{
}

void ABaseCharacter::Walk()
{
}

void ABaseCharacter::Swap(int32 Number)
{
	switch (Number)
	{
		case 1:
		{
			Logger::Print(FString("Swap1!!"));
			break;
		}
		case 2:
		{
			Logger::Print(FString("Swap2!!"));
			break;
		}
		case 3:
		{
			Logger::Print(FString("Swap3!!"));
			break;
		}
		case 4:
		{
			Logger::Print(FString("Swap4!!"));
			break;
		}
	default:
		checkNoEntry();
	}
}

void ABaseCharacter::Attack(bool bPressed)
{
	Logger::Print(FString("Attack!!"));
}

void ABaseCharacter::AttackAssist(bool bPressed)
{
	Logger::Print(FString("AttackAssist!!"));
}

void ABaseCharacter::ActivateSkill(int32 Number, bool bPressed)
{
	Logger::Print(FString("ActivateSkill!!"));
}

void ABaseCharacter::Dodge()
{
	Logger::Print(GetActorForwardVector().CosineAngle2D(GetLastMovementInputVector()));
}

void ABaseCharacter::Draw()
{
	OnDraw.ExecuteIfBound();
}

void ABaseCharacter::Sheath()
{
	OnSheath.ExecuteIfBound();
}

void ABaseCharacter::SetState(ECharacterState CharacterState)
{
	return State->SetState(CharacterState);
}

ECharacterState ABaseCharacter::GetState()
{
	return State->GetState();
}

bool ABaseCharacter::IsState(ECharacterState CharacterState)
{
	return State->IsState(CharacterState);
}

bool ABaseCharacter::IsChangeable(ECharacterState CharacterState)
{
	return State->IsChangeable(CharacterState);
}

bool ABaseCharacter::IsArmed()
{
	return State->IsArmed();
}

bool ABaseCharacter::IsDashable()
{
	return State->IsDashable();
}

bool ABaseCharacter::IsSkillable()
{
	return State->IsSkillable();
}

bool ABaseCharacter::IsAttackable()
{
	return State->IsAttackable();
}

bool ABaseCharacter::IsAttackAssistable()
{
	return State->IsAttackAssistable();
}

bool ABaseCharacter::IsReloadable()
{
	return State->IsReloadable();
}

bool ABaseCharacter::IsSwapable()
{
	return State->IsSwapable();
}

bool ABaseCharacter::IsModeChangeable()
{
	return State->IsModeChangeable();
}

bool ABaseCharacter::IsHitMontagePlayable()
{
	return State->IsHitMontagePlayable();
}

void ABaseCharacter::SetGaitState(EGaitState InGaitState)
{
	State->SetGaitState(InGaitState);
}

EGaitState ABaseCharacter::GetGaitState()
{
	return State->GetGaitState();
}

bool ABaseCharacter::IsSameGaitState(EGaitState InGaitState)
{
	return State->IsSameGaitState(InGaitState);
}

FStat& ABaseCharacter::GetStat(EStat StatType)
{
	return Stat->GetStat(StatType);
}

void ABaseCharacter::SetStat(const FStat& InStat)
{
	Stat->SetStat(InStat);
}

void ABaseCharacter::SetStat(EStat StatType, float Value)
{
	Stat->SetStat(StatType, Value);
}

void ABaseCharacter::AddStat(EStat StatType, float AddValue)
{
	Stat->AddStat(StatType, AddValue);
}

void ABaseCharacter::OnSprint(float Alpha)
{
	GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(WalkSpeed::Run, WalkSpeed::Sprint, Alpha);
}

EHittedDirection ABaseCharacter::CalculateHitFromDirection(AActor* DamageCauser, AController* EventInstigator)
{
	EHittedDirection HittedDirection = EHittedDirection::Any;
	FVector VectorByDamaged;
	FVector DamageCauserLocation;
	FVector Cross;
	float Dot = 0.f;

	if (IsValid(DamageCauser))
	{
		DamageCauserLocation = DamageCauser->GetActorLocation();
	}
	else if (IsValid(EventInstigator))
	{
		DamageCauserLocation = EventInstigator->GetPawn()->GetActorLocation();
	}
	else
	{
		return HittedDirection;
	}

	VectorByDamaged = DamageCauserLocation - GetActorLocation();
	VectorByDamaged = FVector(VectorByDamaged.X, VectorByDamaged.Y, 0.f);
	VectorByDamaged.Normalize();
	
	Dot = FVector::DotProduct(GetActorForwardVector(), VectorByDamaged);
	Cross = FVector::CrossProduct(GetActorForwardVector(), VectorByDamaged);

	if (Dot > FMath::Cos(FMath::DegreesToRadians(45)))	//forward
	{
		HittedDirection = EHittedDirection::Forward;
	}
	else if (Dot < FMath::Cos(FMath::DegreesToRadians(135)))	//backward
	{
		HittedDirection = EHittedDirection::Backward;
	}
	//left, right
	else
	{
		if (Cross.Z > 0)	//right
		{
			HittedDirection = EHittedDirection::Right;
		}
		//left
		else
		{
			HittedDirection = EHittedDirection::Left;
		}
	}

	return HittedDirection;
}
