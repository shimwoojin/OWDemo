// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponSetting.generated.h"

UCLASS(config = WeaponSetting)
class OWDEMOSETTING_API UWeaponSetting : public UObject
{
	GENERATED_BODY()
	
public:
	UWeaponSetting();

	UPROPERTY(config)
		FSoftObjectPath AssaultRifleTracer;

	UPROPERTY(config)
		int32 TestNum;
};
