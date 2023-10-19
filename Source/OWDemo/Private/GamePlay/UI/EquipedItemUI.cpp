// If you want to use this project, mail to ggoggal@gmail.com

#include "EquipedItemUI.h"
#include "ItemUIData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UEquipedItemUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	if (auto* const ItemUIData = Cast<UItemUIData>(ListItemObject))
	{
		SetItemTexture(ItemUIData);
		SetItemName(ItemUIData);
	}
}

void UEquipedItemUI::SetItemTexture(UItemUIData* ItemUIData)
{
	ItemImage->SetBrushFromTexture(ItemUIData->ItemTexture);
}

void UEquipedItemUI::SetItemName(UItemUIData* ItemUIData)
{
	ItemName->SetText(FText::FromString(ItemUIData->ItemName));
}