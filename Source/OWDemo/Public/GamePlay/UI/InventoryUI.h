// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "Blueprint/UserWidget.h"
#include "OWEnums.h"
#include "InventoryUI.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FWeaponInventoryChangeDelegate, EWeaponMode);

UCLASS()
class OWDEMO_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() final;

public:
	void UpdateItems(EItemClassification InItemClassification);

private:
	void OnItemClick(UObject* Object);

public:
	FWeaponInventoryChangeDelegate OnWeaponInventoryChange;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class UListView* ItemList;

protected:
	UPROPERTY()
		TWeakObjectPtr<class ABasePlayer> BasePlayer;
	
	UPROPERTY()
		TWeakObjectPtr<class UInventoryComponent> InventoryManager;
};
