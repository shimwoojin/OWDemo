// If you want to use this project, mail to ggoggal@gmail.com

#include "BTD_GriffonMode.h"
#include "AIController.h"

bool UBTD_GriffonMode::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AGriffon* Griffon = Cast<AGriffon>(OwnerComp.GetAIOwner()->GetPawn());

	if (Griffon)
	{
		return Griffon->GetGriffonMode() == GriffonMode;
	}

	return false;
}