// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "ManagementBase.h"
#include "SkillActivateManagement.generated.h"

UINTERFACE(MinimalAPI)
class USkillActivateManagement : public UManagementBase
{
	GENERATED_BODY()
};

class OWDEMO_API ISkillActivateManagement : public IManagementBase
{
	GENERATED_BODY()

public:
	virtual void OnSkillStartNotify() {}
	virtual void OnSkillEndNotify() {}
};
