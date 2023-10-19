// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "CharacterBaseComponent.h"
#include "GameMake.h"
#include "InventoryManagement.h"
#include "InventoryComponent.generated.h"

class AInventoryItem;

UCLASS()
class OWDEMO_API UInventoryComponent :
	public UCharacterBaseComponent,
	public IInventoryManagement
{
	GENERATED_BODY()
	
public:
	UInventoryComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	bool SwapOnInventory(EItemClassification ItemClassification, int32 First, int32 Second);
	virtual bool AddItemToInventory(AInventoryItem* const WantToAdd) final;
	virtual bool SubItemToInventory(AInventoryItem* const WantToSub, int32 Count) final;
	virtual bool SubItemToInventory(EItemClassification InItemClassification, int32 Index, int32 Count) final;

public:
	TArray<FInventoryItemData>* GetInventory(EItemClassification InItemClassification);
	int32 GetItemCountAtIndex(EItemClassification InItemClassification, int32 Index);

private:
	void Initialize(int32 InInventorySize);
	void LoadData(int32 Slot);
	void SaveData(int32 Slot);
	FString GetSaveSlotName(int32 Slot) { return FString("InventoryItemData") + "_" + FString::FromInt(Slot); }

private:
	UPROPERTY()
		TArray<FInventoryItemData> WeaponInventory;

	UPROPERTY()
		TArray<FInventoryItemData> ArmorInventory;

	UPROPERTY()
		TArray<FInventoryItemData> ConsumptionInventory;

	UPROPERTY()
		TArray<FInventoryItemData> EtcInventory;

	TMap<EItemClassification, TArray<int32>> CountOfItem;

	int32 InventorySize;
};
