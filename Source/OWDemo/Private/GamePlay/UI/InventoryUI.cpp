// If you want to use this project, mail to ggoggal@gmail.com

#include "InventoryUI.h"
#include "BasePlayer.h"
#include "InventoryComponent.h"
#include "Components/ListView.h"
#include "ItemUI.h"
#include "ItemUIData.h"
#include "InventoryItem.h"
#include "BasePlayerController.h"
#include "ModeManageComponent.h"
#include "InventoryItem.h"
#include "BaseWeapon.h"
#include "ItemEquipmentUI.h"
#include "WeaponComponent.h"

void UInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();

	BasePlayer = Cast<ABasePlayer>(GetOwningPlayerPawn());
	InventoryManager = BasePlayer->GetInventoryManager();

	UpdateItems(EItemClassification::Weapon);

	ItemList->OnItemClicked().AddUObject(this, &ThisClass::OnItemClick);
}

void UInventoryUI::UpdateItems(EItemClassification InItemClassification)
{
	if (InventoryManager.Get())
	{
		auto* const Inventory = InventoryManager.Get()->GetInventory(InItemClassification);
		ItemList->ClearListItems();

		for (int i = 0; i < Inventory->Num(); i++)
		{
			UItemUIData* ItemUIData = NewObject<UItemUIData>(this);

			if ((*Inventory)[i].ItemClass)
			{
				AInventoryItem* InventoryItem = Cast<AInventoryItem>((*Inventory)[i].ItemClass->GetDefaultObject());

				ItemUIData->SpawnItemClass = (*Inventory)[i].ItemClass;
				ItemUIData->ItemTexture = InventoryItem->ItemTexture;
				ItemUIData->ItemName = InventoryItem->GetItemName();
				ItemUIData->ItemCount = InventoryManager->GetItemCountAtIndex(InItemClassification, i);
				ItemUIData->DataFrom = EDataFrom::InventoryUI;
				ItemUIData->DataFromIndex = i;
				ItemUIData->SaveSlotName = (*Inventory)[i].ItemUniqueID;
				ItemUIData->SaveSlotIndex = (*Inventory)[i].SaveIndex;
			}

			ItemList->AddItem(ItemUIData);
		}
	}
}

void UInventoryUI::OnItemClick(UObject* Object)
{
	UItemUIData* ItemUIData = Cast<UItemUIData>(Object);
	UModeManageComponent* ModeManager = BasePlayer->GetModeManager();

	if (ItemUIData)
	{
		if (auto* const BasePlayerController = Cast<ABasePlayerController>(BasePlayer->GetController()))
		{
			UItemUIData* MouseGrabbedData = Cast<UItemUIData>(BasePlayerController->GetInventoryItemData());

			if (MouseGrabbedData)
			{
				if (MouseGrabbedData->DataFrom == EDataFrom::EquipmentUI)
				{
					if (ItemUIData->SpawnItemClass == nullptr)
					{
						if (MouseGrabbedData->SpawnItemInstance.Get())
						{
							ABaseWeapon* Weapon = Cast<ABaseWeapon>(MouseGrabbedData->SpawnItemInstance);

							if (InventoryManager.Get())
							{
								InventoryManager->AddItemToInventory(Weapon);
							}

							if (ModeManager)
							{
								ModeManager->DeleteWeaponToSlot(Weapon->GetWeaponMode(), Weapon);
							}

							UpdateItems(EItemClassification::Weapon);

							if (auto* const EquipmentUI = BasePlayerController->GetItemEquipmentUI())
							{
								EquipmentUI->UpdateItems();
							}

							OnWeaponInventoryChange.Broadcast(Weapon->GetWeaponMode());
						}

						BasePlayerController->SetInventoryItemData(nullptr);
					}
					else
					{
						BasePlayerController->SetInventoryItemData(Object);
					}
				}
				else if (MouseGrabbedData->DataFrom == EDataFrom::InventoryUI)
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