// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "BehaviorTree/BTService.h"
#include "BTS_LookTargetSmooth.generated.h"

UCLASS()
class OWDEMO_API UBTS_LookTargetSmooth : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_LookTargetSmooth();

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere)
		float InterpSpeed;
};
