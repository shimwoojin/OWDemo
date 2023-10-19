// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "Blueprint/UserWidget.h"
#include "OWEnums.h"
#include "ItemEquipmentUI.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipmentChangeDeleagate, EWeaponMode);

UCLASS()
class OWDEMO_API UItemEquipmentUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() final;

public:
	void UpdateItems();

private:
	void OnSoldierItemClick(UObject* Object);
	void OnWeaponItemClick(UObject* Object);

	void Internal_OnItemClick(UObject* Object, EWeaponMode InWeaponMode);

public:
	FOnEquipmentChangeDeleagate OnEquipmentChange;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class UListView* SoldierItemList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class UListView* WeaponItemList;

protected:
	UPROPERTY()
		TWeakObjectPtr<class ABasePlayer> BasePlayer;

	UPROPERTY()
		TWeakObjectPtr<class UModeManageComponent> ModeManager;

	UPROPERTY()
		TWeakObjectPtr<class UInventoryComponent> InventoryManager;

	UPROPERTY()
		TWeakObjectPtr<class UWeaponComponent> NormalWeaponMode;

	UPROPERTY()
		TWeakObjectPtr<class UWeaponComponent> SoldierWeaponMode;
};
