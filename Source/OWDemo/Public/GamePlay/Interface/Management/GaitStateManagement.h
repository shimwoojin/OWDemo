// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "ManagementBase.h"
#include "OWEnums.h"
#include "GaitStateManagement.generated.h"

UINTERFACE(MinimalAPI)
class UGaitStateManagement : public UManagementBase
{
	GENERATED_BODY()
};

class OWDEMO_API IGaitStateManagement : public IManagementBase
{
	GENERATED_BODY()

public:
	virtual void SetGaitState(EGaitState InGaitState) {}
	virtual EGaitState GetGaitState() { return EGaitState(); }
	virtual bool IsSameGaitState(EGaitState InGaitState) { return false; }
};
