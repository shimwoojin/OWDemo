// If you want to use this project, mail to ggoggal@gmail.com

#include "InventoryManagement.h"

bool IInventoryManagement::AddItemToInventory(class AInventoryItem* const WantToAdd)
{
	return false;
}

bool IInventoryManagement::SubItemToInventory(class AInventoryItem* const WantToSub, int32 Count)
{
	return false;
}

bool IInventoryManagement::SubItemToInventory(EItemClassification InItemClassification, int32 Index, int32 Count)
{
	return false;
}