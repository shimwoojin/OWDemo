// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "GameFramework/SaveGame.h"
#include "GameMake.h"
#include "InventorySaveGame.generated.h"

UCLASS()
class OWDEMO_API UInventorySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		FString SaveSlotName;

	UPROPERTY()
		int32 SaveSlotIndex;

	UPROPERTY()
		TArray<FInventoryItemData> WeaponInventoryItemData;

	UPROPERTY()
		TArray<FInventoryItemData> ArmorInventoryItemData;

	UPROPERTY()
		TArray<FInventoryItemData> ConsumptionInventoryItemData;

	UPROPERTY()
		TArray<FInventoryItemData> EtcInventoryItemData;

	UPROPERTY()
		TArray<int32> CountOfWeapon;

	UPROPERTY()
		TArray<int32> CountOfArmor;

	UPROPERTY()
		TArray<int32> CountOfConsumption;

	UPROPERTY()
		TArray<int32> CountOfEtc;

	UPROPERTY()
		int32 InventorySize;
};
