// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelSetting.generated.h"

UCLASS(config = LevelSetting)
class OWDEMOSETTING_API ULevelSetting : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(config)
		FSoftObjectPath BossMapGriffon;
};
