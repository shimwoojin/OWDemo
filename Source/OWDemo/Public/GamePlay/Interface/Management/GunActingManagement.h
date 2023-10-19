// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "ManagementBase.h"
#include "GunActingManagement.generated.h"

UINTERFACE(MinimalAPI)
class UGunActingManagement : public UManagementBase
{
	GENERATED_BODY()
};

class OWDEMO_API IGunActingManagement : public IManagementBase
{
	GENERATED_BODY()

public:
	UFUNCTION(Category = GunActing)
		virtual void Reload();

	UFUNCTION(Category = GunActing)
		virtual void ChangeGunTrigger();
};
