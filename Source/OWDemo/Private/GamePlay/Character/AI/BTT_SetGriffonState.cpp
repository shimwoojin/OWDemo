// If you want to use this project, mail to ggoggal@gmail.com

#include "BTT_SetGriffonState.h"
#include "AIController.h"

EBTNodeResult::Type UBTT_SetGriffonState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AGriffon* Griffon = Cast<AGriffon>(OwnerComp.GetAIOwner()->GetPawn());

	if (Griffon)
	{
		Griffon->SetGriffonState(GriffonState);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}