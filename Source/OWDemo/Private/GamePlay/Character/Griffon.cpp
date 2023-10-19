// If you want to use this project, mail to ggoggal@gmail.com

#include "Griffon.h"
#include "Components/CapsuleComponent.h"
#include "GriffonAIController.h"
#include "GriffonAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MontagePlayDataAsset.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BasePlayer.h"

AGriffon::AGriffon()
{
#pragma region AssetSetup
	ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_BLUEPRINT
	(TEXT("AnimBlueprint'/Game/Blueprints/Actors/Enemy/ABP_Griffon.ABP_Griffon_C'"));
	if (ANIM_BLUEPRINT.Succeeded())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(ANIM_BLUEPRINT.Class);
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> GRIFFON_SKIN(TEXT("SkeletalMesh'/Game/Assets/QuadrapedCreatures/Griffon/Meshes/SK_Griffon.SK_Griffon'"));
	if (GRIFFON_SKIN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(GRIFFON_SKIN.Object);
	}

	ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> GRIFFON_MONTAGES(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/Enemy/DA_GriffonMontages.DA_GriffonMontages'"));
	if (GRIFFON_MONTAGES.Succeeded())
	{
		GriffonMontages = GRIFFON_MONTAGES.Object;
	}
#pragma endregion

	CreateDefaultSubobjectAuto(FlyAttackCapsule);

	if (FlyAttackCapsule)
	{
		FlyAttackCapsule->SetupAttachment(GetRootComponent());
		FlyAttackCapsule->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(90.f, 0.f, 0.f));
		FlyAttackCapsule->SetCapsuleHalfHeight(180.f);
		FlyAttackCapsule->SetCapsuleRadius(90.f);
		FlyAttackCapsule->SetGenerateOverlapEvents(false);
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(150.f);
		GetCapsuleComponent()->SetCapsuleRadius(120.f);
	}

	if (GetMesh())
	{
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -150.f));
	}

	AIControllerClass = AGriffonAIController::StaticClass();
	if (AIControllerClass)
	{
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}

	GriffonSpeedMap.Add(EGriffonSpeed::Zero, 0);
	GriffonSpeedMap.Add(EGriffonSpeed::Normal, 400);
	GriffonSpeedMap.Add(EGriffonSpeed::Fast, 800);

	FlyHeight = 400.f;
	StationaryFlyHeight = 300.f;

	TargetBackDistance = 500.f;

	bUseControllerRotationYaw = false;
}

void AGriffon::BeginPlay()
{
	Super::BeginPlay();

	FlyAttackCapsule->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnFlyAttackOverlap);

	SetGriffonSpeed(EGriffonSpeed::Normal);
	SetGriffonState(EGriffonState::Idle);
	SetGriffonMode(EGriffonMode::Ground);

	SetMaxHP(500.f, true);
}

void AGriffon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	OnTickDecideHeight(DeltaSeconds);
}

float AGriffon::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float LocalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	float Percent = GetStat(EStat::CurrentHP).Value / GetStat(EStat::MaxHP).Value;

	if (Percent > .66f)
	{
		if (GriffonMode != EGriffonMode::Ground)
		{
			SetGriffonMode(EGriffonMode::Ground);
		}
	}
	else if (Percent > .33f)
	{
		if (GriffonMode != EGriffonMode::Fly)
		{
			FMontagePlayData JumpToFlyMontage = GriffonMontages->FindMontagePlayData("JumpToFly");
			
			if (JumpToFlyMontage.AnimMontage)
			{
				MontagePlay(JumpToFlyMontage);

				FOnMontageEnded OnMontageEnded;
				OnMontageEnded.BindLambda([this](UAnimMontage* AnimMontage, bool bInterrupted) {
					SetGriffonMode(EGriffonMode::Fly);
					});

				GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded, JumpToFlyMontage.AnimMontage);
			}
		}
	}
	else
	{
		if (GriffonMode != EGriffonMode::StationaryFly)
		{
			SetGriffonMode(EGriffonMode::StationaryFly);
		}
	}

	return LocalDamage;
}

void AGriffon::OnFlyAttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto* const BasePlayer = Cast<ABasePlayer>(OtherActor))
	{
		if (OneTimeDamage.Contains(BasePlayer) == false)
		{
			OneTimeDamage.Add(BasePlayer);
			FDamageEvent DamageEvent;
			OtherActor->TakeDamage(50.f, DamageEvent, GetController(), this);
		}
	}
}

void AGriffon::Attack(bool bPressed)
{
	if (bPressed && GriffonState == EGriffonState::Idle)
	{
		FMontagePlayData AttackMontage;
		TArray<FMontagePlayData> GriffonAttackMontages;
		MontagePlayRate = 1.f;

		switch (GriffonMode)
		{
		case EGriffonMode::Ground:
		{
			GriffonAttackMontages = GriffonMontages->FindAllMontagePlayData("GroundAttack");
			break;
		}
		case EGriffonMode::Fly:
		{
			GriffonAttackMontages = GriffonMontages->FindAllMontagePlayData("FlyAttack");
			MontagePlayRate = FMath::FRandRange(1.5f, 2.f);
			break;
		}
		case EGriffonMode::StationaryFly:
			GriffonAttackMontages = GriffonMontages->FindAllMontagePlayData("StationaryAttack");
			break;
		}

		int32 Max = GriffonAttackMontages.Num();
		int32 Index = 0;

		if (Max > 0)
		{
			Index = UKismetMathLibrary::RandomIntegerInRange(0, Max - 1);
		}
		else if (Max == 0)
		{
			LOG(Warning, TEXT("No AttackMontage In Griffon"));
			return;
		}

		AttackMontage = GriffonAttackMontages[Index];
		AttackMontage.PlayRate = MontagePlayRate;

		if (AttackMontage.AnimMontage)
		{
			MontagePlay(AttackMontage);
			SetGriffonState(EGriffonState::Attack);

			FOnMontageEnded OnMontageEnded;

			OnMontageEnded.BindLambda([this](UAnimMontage* AnimMontage, bool bInterrupted) {

				if (!bInterrupted)
				{
					SetGriffonState(EGriffonState::Idle);
				}

				});

			GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded, AttackMontage.AnimMontage);
		}
	}
}

void AGriffon::StartAttack()
{
	AGriffonAIController* GriffonAIController = Cast<AGriffonAIController>(GetController());

	if (GriffonAIController)
	{
		ABaseCharacter* Target = GriffonAIController->GetTarget();
		if (!Target) return;

		switch (GriffonMode)
		{
		case EGriffonMode::Ground:
		{
			FDamageEvent DamageEvent;
			Target->TakeDamage(GroundAttackDamage, DamageEvent, GetController(), this);
			break;
		}
		case EGriffonMode::Fly:
		{
			FlyAttackStartLocation = GetActorLocation();
			FlyAttackAimLocation = Target->GetActorLocation() + GetActorForwardVector() * TargetBackDistance;
			bTargetLocationSet = true;
			FlyAttackCapsule->SetGenerateOverlapEvents(true);

			AGriffonAIController* AIController = Cast<AGriffonAIController>(GetController());

			if (AIController)
			{
				UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();

				if (BlackboardComponent)
				{
					BlackboardComponent->SetValueAsBool(AGriffonAIController::bLookTargetKey, false);
				}
			}

			break;
		}
		case EGriffonMode::StationaryFly:
		{
			break;
		}
		}
	}
}

void AGriffon::AttackTick(float DeltaTime, float TotalDuration)
{
	AGriffonAIController* GriffonAIController = Cast<AGriffonAIController>(GetController());
	AccumulatedAttackTickTime += (DeltaTime * MontagePlayRate);

	if (GriffonAIController)
	{
		ABaseCharacter* Target = GriffonAIController->GetTarget();
		if (!Target) return;

		switch (GriffonMode)
		{
		case EGriffonMode::Fly:
		{
			if (bTargetLocationSet)
			{
				FVector GriffonLocation = FMath::Lerp(FlyAttackStartLocation, FlyAttackAimLocation, AccumulatedAttackTickTime / TotalDuration);
				SetActorLocation(GriffonLocation, true);
			}
			break;
		}
		case EGriffonMode::StationaryFly:
		{

			break;
		}
		}
	}
}

void AGriffon::EndAttack()
{
	switch (GriffonMode)
	{
	case EGriffonMode::Fly:
	{
		bTargetLocationSet = false;
		AccumulatedAttackTickTime = 0.f;
		FlyAttackCapsule->SetGenerateOverlapEvents(false);
		OneTimeDamage.Empty();

		AGriffonAIController* AIController = Cast<AGriffonAIController>(GetController());

		if (AIController)
		{
			UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();

			if (BlackboardComponent)
			{
				BlackboardComponent->SetValueAsBool(AGriffonAIController::bLookTargetKey, true);
			}
		}
	}
		break;
	case EGriffonMode::StationaryFly:
		break;
	}
}

void AGriffon::SetGriffonMode(EGriffonMode InGriffonMode)
{
	if (auto* const GriffonAnim = Cast<UGriffonAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		GriffonAnim->SetGriffonMode(InGriffonMode);
		GriffonMode = InGriffonMode;

		switch (GriffonMode)
		{
		case EGriffonMode::Ground:
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			break;
		}
		case EGriffonMode::Fly:
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			break;
		}
		case EGriffonMode::StationaryFly:
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			break;
		}
		}
	}
	else
	{
		LOG(Warning, TEXT("Griffon Anim is Weird"));
	}
}

void AGriffon::SetGriffonState(EGriffonState InGriffonState)
{
	if (auto* const GriffonAnim = Cast<UGriffonAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		GriffonAnim->SetGriffonState(InGriffonState);
		GriffonState = InGriffonState;
	}
	else
	{
		LOG(Warning, TEXT("Griffon Anim is Weird"));
	}
}

void AGriffon::SetGriffonSpeed(EGriffonSpeed InGriffonSpeed)
{
	if (auto* const GriffonAnim = Cast<UGriffonAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		GriffonAnim->SetGriffonSpeed(InGriffonSpeed);
		GriffonSpeed = InGriffonSpeed;
		
		switch (GriffonSpeed)
		{
		case EGriffonSpeed::Zero:
		{
			GetCharacterMovement()->MaxWalkSpeed = *GriffonSpeedMap.Find(EGriffonSpeed::Zero);
			break;
		}
		case EGriffonSpeed::Normal:
		{
			GetCharacterMovement()->MaxWalkSpeed = *GriffonSpeedMap.Find(EGriffonSpeed::Normal);
			break;
		}
		case EGriffonSpeed::Fast:
		{
			GetCharacterMovement()->MaxWalkSpeed = *GriffonSpeedMap.Find(EGriffonSpeed::Fast);
			break;
		}
		}
	}
	else
	{
		LOG(Warning, TEXT("Griffon Anim is Weird"));
	}
}

void AGriffon::OnTickDecideHeight(float DeltaSeconds)
{
	FVector GriffonLocation = GetActorLocation();
	bool bLineTrace = true;
	float FlyAimHeight = 0.f;

	switch (GriffonMode)
	{
	case EGriffonMode::Ground:
	{
		bLineTrace = false;
		break;
	}
	case EGriffonMode::Fly:
	{
		FlyAimHeight = FlyHeight;
		break;
	}
	case EGriffonMode::StationaryFly:
	{
		FlyAimHeight = StationaryFlyHeight;
		break;
	}
	}

	if (bLineTrace)
	{
		FHitResult HitResult;
		FCollisionQueryParams CollisionQueryParam;
		CollisionQueryParam.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel
		(
			HitResult,
			GriffonLocation,
			GriffonLocation - FVector(0.f, 0.f, FlyHeight + StationaryFlyHeight),
			ECC_Visibility,
			CollisionQueryParam
		);

		if (HitResult.bBlockingHit)
		{
			FVector GriffonAfterLocation = FMath::VInterpTo(GriffonLocation, HitResult.Location + FVector(0.f, 0.f, FlyAimHeight), DeltaSeconds, 5.f);
			SetActorLocation(GriffonAfterLocation);
		}
	}
}
