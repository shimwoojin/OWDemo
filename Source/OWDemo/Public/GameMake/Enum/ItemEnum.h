#pragma once
#include "OWDemo.h"

UENUM()
enum class EItemClassification : uint8
{
	None,
	Weapon,
	Armor,
	Comsumption,
	Etc,
	Max
};