// If you want to use this project, mail to ggoggal@gmail.com

#include "BTT_SetGriffonMode.h"
#include "AIController.h"

EBTNodeResult::Type UBTT_SetGriffonMode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AGriffon* Griffon = Cast<AGriffon>(OwnerComp.GetAIOwner()->GetPawn());

	if (Griffon)
	{
		if (bRandom)
		{
			EGriffonMode ModeNow = Griffon->GetGriffonMode();
			EGriffonMode NewMode = (EGriffonMode)((FMath::RandRange(1, (int32)EGriffonMode::Max - 1) + (int32)ModeNow) % (int32)EGriffonMode::Max);
			Griffon->SetGriffonMode(NewMode);
			return EBTNodeResult::Succeeded;
		}
		else
		{
			Griffon->SetGriffonMode(GriffonMode);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}