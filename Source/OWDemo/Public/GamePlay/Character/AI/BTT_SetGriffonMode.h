// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Griffon.h"
#include "BTT_SetGriffonMode.generated.h"

UCLASS()
class OWDEMO_API UBTT_SetGriffonMode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere)
		EGriffonMode GriffonMode;

	UPROPERTY(EditAnywhere)
		bool bRandom;
};
