// If you want to use this project, mail to ggoggal@gmail.com

#include "InventoryComponent.h"
#include "InventoryItem.h"
#include "InventorySaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponDataSaveGame.h"

#define MAX_INVENTORY_SIZE 100
#define INVENTORY_SIZE 20

UInventoryComponent::UInventoryComponent()
{
	Initialize(INVENTORY_SIZE);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	LoadData(0);
}

void UInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SaveData(0);
}

bool UInventoryComponent::SwapOnInventory(EItemClassification ItemClassification, int32 First, int32 Second)
{
	SMOOTH_CHECK(First != Second, false);
	SMOOTH_CHECK(First >= 0 || First < INVENTORY_SIZE, false);
	SMOOTH_CHECK(Second >= 0 || Second < INVENTORY_SIZE, false);

	TArray<FInventoryItemData>* SelectedInventory = GetInventory(ItemClassification);

	SelectedInventory->Swap(First, Second);

	if (auto* Count = CountOfItem.Find(ItemClassification))
	{
		Count->Swap(First, Second);
	}

	return true;
}

bool UInventoryComponent::AddItemToInventory(AInventoryItem* const WantToAdd)
{
	SMOOTH_CHECK(WantToAdd, false);

	EItemClassification LocalItemClassification = WantToAdd->GetItemClassification();
	bool bCanMerge = WantToAdd->CanMergeOnInventory();

	auto* const WillBeAddedInventory = GetInventory(LocalItemClassification);
	bool bSuccessAdd = false;

	SMOOTH_CHECK(WillBeAddedInventory, false);

	if (bCanMerge)
	{
		int32 FindIndex = -1;
		for (int i = 0; i < WillBeAddedInventory->Num(); i++)
		{
			if ((*WillBeAddedInventory)[i].ItemClass == WantToAdd->GetClass())
			{
				FindIndex = i;
				break;
			}
		}

		if (FindIndex != INDEX_NONE)
		{
			if (auto* const SpecifiedCountOfItem = CountOfItem.Find(LocalItemClassification))
			{
				++(*SpecifiedCountOfItem)[FindIndex];
				bSuccessAdd = true;
			}
		}
		else
		{
			for (int i = 0; i < WillBeAddedInventory->Num(); i++)
			{
				if ((*WillBeAddedInventory)[i].ItemClass == nullptr)
				{
					(*WillBeAddedInventory)[i].ItemClass = WantToAdd->GetClass();
					(*WillBeAddedInventory)[i].ItemUniqueID = WantToAdd->GetInventoryItemUniqueID();

					if (auto* const SpecifiedCountOfItem = CountOfItem.Find(LocalItemClassification))
					{
						(*SpecifiedCountOfItem)[i] = 1;
					}
					else
					{
						LOG(Warning, TEXT("Don't Increase CountOfItem, Yet You Get Item"))
					}

					bSuccessAdd = true;
					break;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < WillBeAddedInventory->Num(); i++)
		{
			if ((*WillBeAddedInventory)[i].ItemClass == nullptr)
			{
				(*WillBeAddedInventory)[i].ItemClass = WantToAdd->GetClass();
				(*WillBeAddedInventory)[i].ItemUniqueID = WantToAdd->GetInventoryItemUniqueID();

				if (auto* const SpecifiedCountOfItem = CountOfItem.Find(LocalItemClassification))
				{
					(*SpecifiedCountOfItem)[i] = 1;
				}
				else
				{
					LOG(Warning, TEXT("Don't Increase CountOfItem, Yet You Get Item"))
				}

				bSuccessAdd = true;
				break;
			}
		}
	}

	WantToAdd->SaveData();

	return bSuccessAdd;
}

bool UInventoryComponent::SubItemToInventory(AInventoryItem* const WantToSub, int32 Count)
{
	SMOOTH_CHECK(WantToSub, false);

	EItemClassification LocalItemClassification = WantToSub->GetItemClassification();
	bool bCanMerge = WantToSub->CanMergeOnInventory();

	auto* const WillBeSubbedInventory = GetInventory(LocalItemClassification);
	bool bSuccessSub = false;

	SMOOTH_CHECK(WillBeSubbedInventory, false);

	int32 FindIndex = -1;
	for (int i = 0; i < WillBeSubbedInventory->Num(); i++)
	{
		if ((*WillBeSubbedInventory)[i].ItemClass == WantToSub->GetClass())
		{
			FindIndex = i;
			break;
		}
	}

	if (bCanMerge)
	{
		if (FindIndex != INDEX_NONE)
		{
			if (auto* const SpecifiedCountOfItem = CountOfItem.Find(LocalItemClassification))
			{
				(*SpecifiedCountOfItem)[FindIndex] -= Count;
				bSuccessSub = true;

				if ((*SpecifiedCountOfItem)[FindIndex] <= 0)
				{
					WillBeSubbedInventory->RemoveAt(FindIndex);
					(*SpecifiedCountOfItem)[FindIndex] = 0;
				}
			}
		}
	}
	else
	{
		(*WillBeSubbedInventory)[FindIndex].ItemClass = nullptr;
		(*WillBeSubbedInventory)[FindIndex].ItemUniqueID = "";
		
		if (auto* const SpecifiedCountOfItem = CountOfItem.Find(LocalItemClassification))
		{
			(*SpecifiedCountOfItem)[FindIndex] = 0;

			bSuccessSub = true;
		}
	}

	return bSuccessSub;
}

bool UInventoryComponent::SubItemToInventory(EItemClassification InItemClassification, int32 Index, int32 Count)
{
	auto* Inventory = GetInventory(InItemClassification);

	bool IsValidIndex = Inventory->IsValidIndex(Index);

	if (IsValidIndex)
	{
		auto* ItemCount = CountOfItem.Find(InItemClassification);

		if ((*ItemCount)[Index] > Count)
		{
			(*ItemCount)[Index] = (*ItemCount)[Index] - Count;
		}
		else
		{
			FInventoryItemData InventoryItemData_Null;
			(*Inventory)[Index] = InventoryItemData_Null;
			(*ItemCount)[Index] = 0;
		}
	}

	return true;
}

TArray<FInventoryItemData>* UInventoryComponent::GetInventory(EItemClassification InItemClassification)
{
	switch (InItemClassification)
	{
	case EItemClassification::Weapon:
		return &WeaponInventory;
		break;
	case EItemClassification::Armor:
		return &ArmorInventory;
		break;
	case EItemClassification::Comsumption:
		return &ConsumptionInventory;
		break;
	case EItemClassification::Etc:
		return &EtcInventory;
		break;
	default:
	{
		checkNoEntry();
		break;
	}
	}

	LOG(Warning, TEXT("Wrong Inventory Returned!"));
	return nullptr;
}

int32 UInventoryComponent::GetItemCountAtIndex(EItemClassification InItemClassification, int32 Index)
{
	if (auto* const ClassifiedItemArray = CountOfItem.Find(InItemClassification))
	{
		if (ClassifiedItemArray->IsValidIndex(Index))
		{
			return (*ClassifiedItemArray)[Index];
		}
	}

	return INDEX_NONE;
}

void UInventoryComponent::Initialize(int32 InInventorySize)
{
	if (InInventorySize < 0 || InInventorySize > MAX_INVENTORY_SIZE)
	{
		LOG(Error, TEXT("Wrong Inventory Size is Tried To Allocated!"));
	}

	InventorySize = InInventorySize;

	for (int i = (int32)(EItemClassification::None) + 1; i < (int32)EItemClassification::Max; i++)
	{
		TArray<FInventoryItemData>* NewInventory = GetInventory(static_cast<EItemClassification>(i));
		SMOOTH_CHECK(NewInventory);
		NewInventory->AddDefaulted(InventorySize);
		TArray<int32> NewInventorySize;
		NewInventorySize.AddDefaulted(InventorySize);
		CountOfItem.Add(static_cast<EItemClassification>(i), NewInventorySize);
	}
}

void UInventoryComponent::LoadData(int32 Slot)
{
	if (UGameplayStatics::DoesSaveGameExist(GetSaveSlotName(Slot), Slot))
	{
		UInventorySaveGame* SaveGameInstance = Cast<UInventorySaveGame>(UGameplayStatics::CreateSaveGameObject(UInventorySaveGame::StaticClass()));

		if (SaveGameInstance)
		{
			SaveGameInstance = Cast<UInventorySaveGame>(UGameplayStatics::LoadGameFromSlot(GetSaveSlotName(Slot), Slot));

			WeaponInventory = MoveTemp(SaveGameInstance->WeaponInventoryItemData);
			ArmorInventory = MoveTemp(SaveGameInstance->ArmorInventoryItemData);
			ConsumptionInventory = MoveTemp(SaveGameInstance->ConsumptionInventoryItemData);
			EtcInventory = MoveTemp(SaveGameInstance->EtcInventoryItemData);

			InventorySize = SaveGameInstance->InventorySize;
			
			CountOfItem[EItemClassification::Weapon] = MoveTemp(SaveGameInstance->CountOfWeapon);
			CountOfItem[EItemClassification::Armor] = MoveTemp(SaveGameInstance->CountOfArmor);
			CountOfItem[EItemClassification::Comsumption] = MoveTemp(SaveGameInstance->CountOfConsumption);
			CountOfItem[EItemClassification::Etc] = MoveTemp(SaveGameInstance->CountOfEtc);
		}
	}
}

void UInventoryComponent::SaveData(int32 Slot)
{
	UInventorySaveGame* SaveGameInstance = Cast<UInventorySaveGame>(UGameplayStatics::CreateSaveGameObject(UInventorySaveGame::StaticClass()));

	for (const auto& ele : WeaponInventory)	//@TODO: OtherInventory too,,
	{
		if (ele.ItemClass != nullptr)
		{
			UWeaponDataSaveGame* WeaponDataSaveGame = Cast<UWeaponDataSaveGame>(UGameplayStatics::CreateSaveGameObject(UWeaponDataSaveGame::StaticClass()));

			if (WeaponDataSaveGame)
			{
				WeaponDataSaveGame->SaveSlotName = ele.ItemUniqueID;
				WeaponDataSaveGame->SaveSlotIndex = 0;

				UGameplayStatics::SaveGameToSlot(WeaponDataSaveGame, WeaponDataSaveGame->SaveSlotName, WeaponDataSaveGame->SaveSlotIndex);
			}
		}
	}

	if (SaveGameInstance)
	{
		SaveGameInstance->SaveSlotName = GetSaveSlotName(Slot);
		SaveGameInstance->SaveSlotIndex = Slot;

		SaveGameInstance->WeaponInventoryItemData = MoveTemp(WeaponInventory);
		SaveGameInstance->ArmorInventoryItemData = MoveTemp(ArmorInventory);
		SaveGameInstance->ConsumptionInventoryItemData = MoveTemp(ConsumptionInventory);
		SaveGameInstance->EtcInventoryItemData = MoveTemp(EtcInventory);

		SaveGameInstance->InventorySize = InventorySize;

		SaveGameInstance->CountOfWeapon = MoveTemp(*CountOfItem.Find(EItemClassification::Weapon));
		SaveGameInstance->CountOfArmor = MoveTemp(*CountOfItem.Find(EItemClassification::Armor));
		SaveGameInstance->CountOfConsumption = MoveTemp(*CountOfItem.Find(EItemClassification::Comsumption));
		SaveGameInstance->CountOfEtc = MoveTemp(*CountOfItem.Find(EItemClassification::Etc));

		UGameplayStatics::SaveGameToSlot(SaveGameInstance, GetSaveSlotName(Slot), Slot);
	}
}
