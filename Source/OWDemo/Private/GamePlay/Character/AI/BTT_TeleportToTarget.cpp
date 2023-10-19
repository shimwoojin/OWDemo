// If you want to use this project, mail to ggoggal@gmail.com

#include "BTT_TeleportToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GriffonAIController.h"

EBTNodeResult::Type UBTT_TeleportToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AGriffonAIController* GriffonAIController = Cast<AGriffonAIController>(OwnerComp.GetAIOwner());
	SMOOTH_CHECK(GriffonAIController, EBTNodeResult::Failed);
	APawn* ControlledPawn = GriffonAIController->GetPawn();
	SMOOTH_CHECK(ControlledPawn, EBTNodeResult::Failed);

	UBlackboardComponent* BlackboardComponent = GriffonAIController->GetBlackboardComponent();

	if (BlackboardComponent)
	{
		UObject* Target = BlackboardComponent->GetValueAsObject(AGriffonAIController::TargetKey);
		AActor* Target_Actor = Cast<AActor>(Target);
		FVector VectorToTeleport;

		if (bBack)
		{
			VectorToTeleport = -(Target_Actor->GetActorForwardVector()) * DistanceWithTarget;
		}
		else
		{
			VectorToTeleport = Target_Actor->GetActorForwardVector() * DistanceWithTarget;
		}

		FVector TeleportAimLocation = Target_Actor->GetActorLocation() + VectorToTeleport;
		FRotator ControlledPawnAimRotation = Target_Actor->GetActorRotation();

		ControlledPawn->SetActorLocationAndRotation(TeleportAimLocation, ControlledPawnAimRotation);
	}

	return EBTNodeResult::Succeeded;
}