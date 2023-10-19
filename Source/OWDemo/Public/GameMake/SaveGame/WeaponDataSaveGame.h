// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "GameFramework/SaveGame.h"
#include "WeaponDataSaveGame.generated.h"

UCLASS()
class OWDEMO_API UWeaponDataSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
		FString SaveSlotName;

	UPROPERTY(EditDefaultsOnly)
		int32 SaveSlotIndex;

	UPROPERTY(EditDefaultsOnly)
		int32 Upgrade;
};
