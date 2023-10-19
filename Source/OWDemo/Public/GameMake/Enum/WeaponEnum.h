#pragma once
#include "OWDemo.h"

UENUM(BlueprintType)
enum class EWeaponMode : uint8
{
	None,
	Soldier,
	Weapon
};

UENUM(BlueprintType)
enum class ESoldierWeaponType : uint8
{
	None,
	AssaultRifle,
	SniperRifle,
	Pistol,
	ShotGun,
	Knife,
	Grenade
};

UENUM(BlueprintType)
enum class ENormalWeaponType : uint8
{
	None,
	TwoHandSword,
	OneHandSword,
	Shield,
	Spear,
	Throwing,
	Arrow,
	Staff
};

UENUM(BlueprintType)
enum class EGunTriggerGroup : uint8
{
	SingleFire,
	AutoFire
};

UENUM(BlueprintType)
enum class EElementalType : uint8
{
	Water,
	Thunder,
	Max
};