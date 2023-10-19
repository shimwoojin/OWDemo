// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "WeaponEnum.h"
#include "ManagementBase.h"
#include "ModeManagement.generated.h"

UINTERFACE(MinimalAPI)
class UModeManagement : public UManagementBase
{
	GENERATED_BODY()
};

class OWDEMO_API IModeManagement : public IManagementBase
{
	GENERATED_BODY()

public:
	UFUNCTION(category = PlayerInterface)
		virtual void ChangeMode();

	UFUNCTION(category = PlayerInterface)
		virtual void ChangeSkin(EWeaponMode WeaponMode);

	UFUNCTION(category = PlayerInterface)
		virtual void ChangeModeNone();
};
