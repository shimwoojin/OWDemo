// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "Camera/CameraShake.h"
#include "GunShotCameraShake.generated.h"

UCLASS()
class OWDEMO_API UGunShotCameraShake : public UMatineeCameraShake
{
	GENERATED_BODY()
	
public:
	UGunShotCameraShake();
};
