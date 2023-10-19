// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Attack.generated.h"

UCLASS()
class OWDEMO_API UBTT_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_Attack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
