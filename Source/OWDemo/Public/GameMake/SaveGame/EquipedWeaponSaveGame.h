// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "GameFramework/SaveGame.h"
#include "ItemStructure.h"
#include "EquipedWeaponSaveGame.generated.h"

UCLASS()
class OWDEMO_API UEquipedWeaponSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		FString SaveGameSlotName;

	UPROPERTY()
		int32 SaveGameSlotIndex;

	UPROPERTY()
		TArray<FInventoryItemData> Weapons;
};
