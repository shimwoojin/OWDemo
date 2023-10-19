// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "UObject/Interface.h"
#include "OWEnums.h"
#include "InventoryManagement.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryManagement : public UInterface
{
	GENERATED_BODY()
};

class OWDEMO_API IInventoryManagement
{
	GENERATED_BODY()

public:
	virtual bool AddItemToInventory(class AInventoryItem* const WantToAdd);
	virtual bool SubItemToInventory(class AInventoryItem* const WantToSub, int32 Count);
	virtual bool SubItemToInventory(EItemClassification InItemClassification, int32 Index, int32 Count);
};
