// If you want to use this project, mail to ggoggal@gmail.com

#include "ItemUIData.h"
#include "Engine/Texture2D.h"

UTexture2D* UItemUIData::Empty_Image = nullptr;

UItemUIData::UItemUIData()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> EMPTY_IMAGE
	(TEXT("Texture2D'/Game/Assets/Textures/Empty.Empty'"));
	if (EMPTY_IMAGE.Succeeded())
	{
		Empty_Image = EMPTY_IMAGE.Object;
	}

	SpawnItemClass = nullptr;
	SpawnItemInstance = nullptr;
	ItemTexture = Empty_Image;
	ItemName = "Empty";
	ItemCount = 0;
	DataFrom = EDataFrom::Anonymous;
	SaveSlotName = "ItemUIData";
	SaveSlotIndex = 0;
}