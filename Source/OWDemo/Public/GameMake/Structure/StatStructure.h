#pragma once

#include "OWDemo.h"
#include "StatEnum.h"
#include "StatStructure.generated.h"

USTRUCT(BlueprintType)
struct FStat
{
	GENERATED_BODY()

public:
	static FStat StatNone;

public:
	FStat() 
	{
		StatType = EStat::None;
		Value = 0.f;
	}
	
	FStat(EStat InStatType, float InValue)
		: StatType(InStatType), Value(InValue)
	{

	}

	bool operator==(const FStat& OtherHand)
	{
		if (this->StatType == OtherHand.StatType)
		{
			return true;
		}
		return false;
	}

	bool operator==(EStat OtherStatType)
	{
		if (this->StatType == OtherStatType)
		{
			return true;
		}
		return false;
	}

	bool IsValidStat() { return StatType != EStat::None; }

public:
	EStat StatType;
	float Value;
};
