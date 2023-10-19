// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "GameMake.h"
#include "ManagementBase.h"
#include "StatManagement.generated.h"

UINTERFACE(MinimalAPI)
class UStatManagement : public UManagementBase
{
	GENERATED_BODY()
};

class OWDEMO_API IStatManagement : public IManagementBase
{
	GENERATED_BODY()

public:
	virtual FStat& GetStat(EStat StatType);
	virtual void SetStat(const FStat& Stat);
	virtual void SetStat(EStat StatType, float Value);
	virtual void AddStat(EStat StatType, float AddValue);
};
