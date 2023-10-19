// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "UObject/NoExportTypes.h"
#include "ItemUIData.generated.h"

UENUM()
enum class EDataFrom : uint8
{
	Anonymous,
	InventoryUI,
	EquipmentUI
};

UCLASS()
class OWDEMO_API UItemUIData : public UObject
{
	GENERATED_BODY()
	
public:
	UItemUIData();

public:
	static class UTexture2D* Empty_Image;

public:
	UPROPERTY(EditAnywhere)
		UClass* SpawnItemClass;

	UPROPERTY(EditAnywhere)
		TWeakObjectPtr<class AInventoryItem> SpawnItemInstance;

	UPROPERTY(EditAnywhere)
		UTexture2D* ItemTexture;

	UPROPERTY(EditAnywhere)
		FString ItemName;

	UPROPERTY(EditAnywhere)
		int32 ItemCount;

	UPROPERTY(EditAnywhere)
		EDataFrom DataFrom;

	UPROPERTY(EditAnywhere)
		int32 DataFromIndex;

	UPROPERTY(EditAnywhere)
		FString SaveSlotName;

	UPROPERTY(EditAnywhere)
		int32 SaveSlotIndex;
};
