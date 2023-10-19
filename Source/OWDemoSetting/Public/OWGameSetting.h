#pragma once

#include "CoreMinimal.h"

#define ECollisionChannel_Character ECC_GameTraceChannel1
#define ECollisionChannel_Weapon ECC_GameTraceChannel2
#define ECollisionChannel_Bullet ECC_GameTraceChannel3
#define ECollisionChannel_Arrow ECC_GameTraceChannel4
#define ECollisionChannel_Magic ECC_GameTraceChannel5

class OWDEMOSETTING_API OWCollision
{
public:
	static FName const PresetName_NoCollision;
	static FName const PresetName_Weapon;
	static FName const PresetName_Character;
	static FName const PresetName_Bullet;
	static FName const PresetName_Arrow;
	static FName const PresetName_Magic;
	static FName const PresetName_Enemy;
	static FName const PresetName_InventoryItem;
	static FName const PresetName_PlayerTrigger;
	static FName const PresetName_InteractableItem;
};