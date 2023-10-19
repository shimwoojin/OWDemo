// If you want to use this project, mail to ggoggal@gmail.com

#include "BTT_Attack.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "ActingManagement.h"

UBTT_Attack::UBTT_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (ControllingPawn)
	{
		if (auto* const ActingManaged = Cast<IActingManagement>(ControllingPawn))
		{
			ActingManaged->Attack(true);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}