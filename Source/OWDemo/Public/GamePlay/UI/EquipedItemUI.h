// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "EquipedItemUI.generated.h"

UCLASS()
class OWDEMO_API UEquipedItemUI : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) final;

	void SetItemTexture(class UItemUIData* ItemUIData);
	void SetItemName(class UItemUIData* ItemUIData);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class UImage* ItemImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class UTextBlock* ItemName;
};
