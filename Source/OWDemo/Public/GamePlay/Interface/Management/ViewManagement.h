// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "ManagementBase.h"
#include "ViewManagement.generated.h"

UINTERFACE(MinimalAPI)
class UViewManagement : public UManagementBase
{
	GENERATED_BODY()
};

class OWDEMO_API IViewManagement : public IManagementBase
{
	GENERATED_BODY()

public:
	UFUNCTION(Category = View)
		virtual void ToggleCameraAngle();

	UFUNCTION(Category = View)
		virtual void CameraShake(TSubclassOf<class UMatineeCameraShake> CameraShakeClass);
};
