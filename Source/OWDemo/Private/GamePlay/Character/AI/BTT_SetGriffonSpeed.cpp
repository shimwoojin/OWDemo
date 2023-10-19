// If you want to use this project, mail to ggoggal@gmail.com

#include "BTT_SetGriffonSpeed.h"
#include "AIController.h"

EBTNodeResult::Type UBTT_SetGriffonSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AGriffon* Griffon = Cast<AGriffon>(OwnerComp.GetAIOwner()->GetPawn());

	if (Griffon)
	{
		if (bRandom)
		{
			int32 RandomNumber = FMath::RandRange(0, static_cast<int32>(EGriffonSpeed::Max) - 1);
			if (RandomNumber == static_cast<int32>(Griffon->GetGriffonSpeed()))
			{
				++RandomNumber;
			}

			Griffon->SetGriffonSpeed(static_cast<EGriffonSpeed>(RandomNumber));
		}
		else
		{
			Griffon->SetGriffonSpeed(GriffonSpeed);
		}
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}