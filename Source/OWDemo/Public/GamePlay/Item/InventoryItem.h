// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "Item.h"
#include "OWEnums.h"
#include "InventoryManagement.h"
#include "InventoryItem.generated.h"

class ABaseCharacter;

UCLASS()
class OWDEMO_API AInventoryItem :
	public AItem,
	public IInventoryManagement
{
	GENERATED_BODY()
	
public:
	AInventoryItem();
	
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

public:

	UFUNCTION()
		void PickUp();
	
	//InventoryManagement
	virtual bool AddItemToInventory(AInventoryItem* const WantToAdd) final;
	FString GetInventoryItemUniqueID();
	FString SetInventoryItemUniqueID(FString InUniqueID);

public:
	bool CanMergeOnInventory() { return bCanMerge; }
	void SetCanMergeOnInventory(bool InbCanMerge) { bCanMerge = InbCanMerge; }
	void SetItemClassification(EItemClassification InItemClassification) { ItemClassification = InItemClassification; }
	EItemClassification GetItemClassification() { return ItemClassification; }

	bool IsOwnedByPlayer() { return bOwnedByPlayer; }
	void SetOwned(bool InbOwnedByPlayer) { bOwnedByPlayer = InbOwnedByPlayer; }

	virtual bool LoadSaveData(FString InSaveSlotName, int32 InSaveSlotIndex) { return false; }
	virtual void SaveData() {}

public:
	UPROPERTY(EditDefaultsOnly)
		class UTexture2D* ItemTexture;

	static class USoundBase* PickUpSound;

private:
	FString UniqueID;
	bool bCanMerge;
	bool bOwnedByPlayer;
	EItemClassification ItemClassification;

	TWeakObjectPtr<class ABasePlayer> Player;
};
