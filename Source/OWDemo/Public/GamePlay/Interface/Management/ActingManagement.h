// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "ManagementBase.h"
#include "ActingManagement.generated.h"

UINTERFACE(MinimalAPI)
class UActingManagement : public UManagementBase
{
	GENERATED_BODY()
};

class OWDEMO_API IActingManagement : public IManagementBase
{
	GENERATED_BODY()

public:

	UFUNCTION(category = Acting)
		virtual void Dash(bool bPressed);

	UFUNCTION(category = Acting)
		virtual void OnDashNotify();

	UFUNCTION(category = Acting)
		virtual void Attack(bool bPressed);

	UFUNCTION(category = Acting)
		virtual void AttackAssist(bool bPressed);

	UFUNCTION(category = Acting)
		virtual void ActivateSkill(int32 Number, bool bPressed);
};
