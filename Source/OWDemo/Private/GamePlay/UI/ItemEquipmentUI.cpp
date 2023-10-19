// If you want to use this project, mail to ggoggal@gmail.com

#include "ItemEquipmentUI.h"
#include "Components/ListView.h"
#include "ItemUIData.h"
#include "BasePlayer.h"
#include "WeaponComponent.h"
#include "ModeManageComponent.h"
#include "BaseWeapon.h"
#include "BasePlayerController.h"
#include "InventoryComponent.h"
#include "InventoryUI.h"

void UItemEquipmentUI::NativeConstruct()
{
	Super::NativeConstruct();

	BasePlayer = Cast<ABasePlayer>(GetOwningPlayerPawn());
	ModeManager = BasePlayer->GetModeManager();
	InventoryManager = BasePlayer->GetInventoryManager();

	if (ModeManager.Get())
	{
		NormalWeaponMode = ModeManager->GetWeaponModeComponent();
		SoldierWeaponMode = ModeManager->GetSoldierModeComponent();
	}

	SoldierItemList->OnItemClicked().AddUObject(this, &ThisClass::OnSoldierItemClick);
	WeaponItemList->OnItemClicked().AddUObject(this, &ThisClass::OnWeaponItemClick);

	UpdateItems();
}

void UItemEquipmentUI::OnSoldierItemClick(UObject* Object)
{
	Internal_OnItemClick(Object, EWeaponMode::Soldier);
}

void UItemEquipmentUI::OnWeaponItemClick(UObject* Object)
{
	Internal_OnItemClick(Object, EWeaponMode::Weapon);
}

void UItemEquipmentUI::Internal_OnItemClick(UObject* Object, EWeaponMode InWeaponMode)
{
	UItemUIData* ItemUIData = Cast<UItemUIData>(Object);

	if (ItemUIData)
	{
		if (auto* const BasePlayerController = Cast<ABasePlayerController>(BasePlayer->GetController()))
		{
			UListView* SelectedItemList = (InWeaponMode == EWeaponMode::Soldier ? SoldierItemList : WeaponItemList);
			int32 Index = SelectedItemList->GetIndexForItem(Object);
			UItemUIData* MouseGrabbedData = Cast<UItemUIData>(BasePlayerController->GetInventoryItemData());
			UClass* BeforeItemClass = ItemUIData->SpawnItemClass;

			if (MouseGrabbedData != nullptr)
			{
				if (MouseGrabbedData->DataFrom == EDataFrom::InventoryUI)
				{
					if (auto* const SelectedItemDefault = Cast<ABaseWeapon>(MouseGrabbedData->SpawnItemClass->GetDefaultObject()))
					{
						if (SelectedItemDefault->GetWeaponMode() == InWeaponMode)
						{
							ABaseWeapon* BaseWeapon = GetWorld()->SpawnActor<ABaseWeapon>(MouseGrabbedData->SpawnItemClass);

							if (BaseWeapon)
							{
								BaseWeapon->WeaponInitialize(BasePlayer.Get());
								BaseWeapon->AttachTo(BasePlayer.Get(), false);
								BaseWeapon->LoadSaveData(MouseGrabbedData->SaveSlotName, MouseGrabbedData->SaveSlotIndex);

								if (ModeManager.Get())
								{
									if (BeforeItemClass)
									{
										ModeManager->DeleteWeaponToSlot(InWeaponMode, Index);
									}

									ModeManager->InsertWeaponToSlot(InWeaponMode, BaseWeapon, Index);

									UpdateItems();

									if (InventoryManager.Get())
									{
										InventoryManager->SubItemToInventory(SelectedItemDefault, 1);
										
										if (BeforeItemClass)
										{
											InventoryManager->AddItemToInventory(Cast<AInventoryItem>(BeforeItemClass->GetDefaultObject()));
										}

										BasePlayerController->GetInventoryUI()->UpdateItems(SelectedItemDefault->GetItemClassification());
										OnEquipmentChange.Broadcast(InWeaponMode);

										BasePlayerController->SetInventoryItemData(nullptr);
									}
								}
							}
						}
					}
				}
				else if(MouseGrabbedData->DataFrom == EDataFrom::EquipmentUI)
				{
					BasePlayerController->SetInventoryItemData(nullptr);
				}
				else
				{
					BasePlayerController->SetInventoryItemData(Object);
				}
			}
			else
			{
				BasePlayerController->SetInventoryItemData(Object);
			}
		}
	}
}

void UItemEquipmentUI::UpdateItems()
{
	if (NormalWeaponMode.Get())
	{
		TArray<ABaseWeapon*> WeaponArray = NormalWeaponMode.Get()->GetWeapons();
		int32 WeaponCount = WeaponArray.Num();

		WeaponItemList->ClearListItems();

		for (int i = 0; i < WeaponCount; i++)
		{
			UItemUIData* ItemUIData = NewObject<UItemUIData>(this);

			if (WeaponArray[i] != nullptr)
			{
				ItemUIData->SpawnItemClass = WeaponArray[i]->GetClass();
				ItemUIData->SpawnItemInstance = WeaponArray[i];
				ItemUIData->ItemName = WeaponArray[i]->GetItemName();
				ItemUIData->ItemTexture = WeaponArray[i]->ItemTexture;
				ItemUIData->DataFrom = EDataFrom::EquipmentUI;
				ItemUIData->DataFromIndex = i;
				ItemUIData->SaveSlotName = WeaponArray[i]->GetInventoryItemUniqueID();
			}

			WeaponItemList->AddItem(ItemUIData);
		}
	}

	if (SoldierWeaponMode.Get())
	{
		TArray<ABaseWeapon*> WeaponArray = SoldierWeaponMode.Get()->GetWeapons();
		int32 WeaponCount = WeaponArray.Num();

		SoldierItemList->ClearListItems();

		for (int i = 0; i < WeaponCount; i++)
		{
			UItemUIData* ItemUIData = NewObject<UItemUIData>(this);

			if (WeaponArray[i] != nullptr)
			{
				ItemUIData->SpawnItemClass = WeaponArray[i]->GetClass();
				ItemUIData->SpawnItemInstance = WeaponArray[i];
				ItemUIData->ItemName = WeaponArray[i]->GetItemName();
				ItemUIData->ItemTexture = WeaponArray[i]->ItemTexture;
				ItemUIData->DataFrom = EDataFrom::EquipmentUI;
				ItemUIData->SaveSlotName = WeaponArray[i]->GetInventoryItemUniqueID();
			}

			SoldierItemList->AddItem(ItemUIData);
		}
	}
}