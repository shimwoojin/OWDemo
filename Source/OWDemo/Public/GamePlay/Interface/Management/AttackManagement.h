// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "ManagementBase.h"
#include "AttackManagement.generated.h"

UINTERFACE(MinimalAPI)
class UAttackManagement : public UManagementBase
{
	GENERATED_BODY()
};

class OWDEMO_API IAttackManagement : public IManagementBase
{
	GENERATED_BODY()

public:

	UFUNCTION(Category = Attack)
		virtual void StartAttack();

	UFUNCTION(Category = Attack)
		virtual void EndAttack();

	UFUNCTION(Category = Attack)
		virtual void AttackTick(float DeltaTime, float TotalDuration);
};
