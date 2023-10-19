// If you want to use this project, mail to ggoggal@gmail.com

#include "BTS_LookTargetSmooth.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseEnemyController.h"
#include "BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"

UBTS_LookTargetSmooth::UBTS_LookTargetSmooth()
{
	InterpSpeed = 50.f;
	Interval = .1f;
	RandomDeviation = 0.0f;
}

void UBTS_LookTargetSmooth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* const ControllingPawn = AIController->GetPawn();

	if (AIController && ControllingPawn)
	{
		UBlackboardComponent* BlackBoardComponent = AIController->GetBlackboardComponent();

		if (BlackBoardComponent)
		{
			UObject* ObjectTarget = BlackBoardComponent->GetValueAsObject(ABaseEnemyController::TargetKey);
			bool bLookTarget = BlackBoardComponent->GetValueAsBool(ABaseEnemyController::bLookTargetKey);

			if (bLookTarget)
			{
				if (auto* const CharacterTarget = Cast<ABaseCharacter>(ObjectTarget))
				{
					FVector TargetLocation = CharacterTarget->GetActorLocation();
					FVector SelfLocation = ControllingPawn->GetActorLocation();
					FRotator AimRotator = UKismetMathLibrary::FindLookAtRotation(SelfLocation, TargetLocation);
					FRotator AimRotatorZ = FRotator(0.f, AimRotator.Yaw, 0.f);

					FRotator AimRotatorFinal = FMath::RInterpTo(ControllingPawn->GetActorRotation(), AimRotatorZ, DeltaSeconds, InterpSpeed);

					ControllingPawn->SetActorRotation(AimRotatorFinal);
				}
			}
		}
	}
}