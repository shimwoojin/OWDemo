// If you want to use this project, mail to ggoggal@gmail.com

#include "ItemUI.h"
#include "ItemUIData.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UItemUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	SetData(ListItemObject);
}

void UItemUI::SetData(UObject* ListItemObject)
{
	UItemUIData* ItemUIData = Cast<UItemUIData>(ListItemObject);

	if (ItemUIData)
	{
		ItemImage->SetBrushFromTexture(ItemUIData->ItemTexture);
		ItemName->SetText(FText::FromString(ItemUIData->ItemName));
		ItemCount->SetText(FText::FromString(FString::FromInt(ItemUIData->ItemCount)));
	}
}