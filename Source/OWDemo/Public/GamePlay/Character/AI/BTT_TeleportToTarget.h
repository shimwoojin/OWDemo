// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_TeleportToTarget.generated.h"

UCLASS()
class OWDEMO_API UBTT_TeleportToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere)
		bool bBack;

	UPROPERTY(EditAnywhere)
		float DistanceWithTarget;
};
