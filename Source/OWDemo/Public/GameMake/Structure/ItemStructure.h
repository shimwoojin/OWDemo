#pragma once
#include "OWDemo.h"
#include "InventoryItem.h"
#include "ItemStructure.generated.h"

USTRUCT()
struct FInventoryItemData
{
	GENERATED_BODY()

public:
	FInventoryItemData()
		: ItemClass(nullptr), ItemUniqueID("None"), SaveIndex(0)
	{
	}

	UPROPERTY()
		TSubclassOf<AInventoryItem> ItemClass;

	UPROPERTY()
		FString ItemUniqueID;

	UPROPERTY()
		int32 SaveIndex;
};