// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "BehaviorTree/BTDecorator.h"
#include "Griffon.h"
#include "BTD_GriffonMode.generated.h"

UCLASS()
class OWDEMO_API UBTD_GriffonMode : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere, Category = OnMode)
		EGriffonMode GriffonMode;
};
