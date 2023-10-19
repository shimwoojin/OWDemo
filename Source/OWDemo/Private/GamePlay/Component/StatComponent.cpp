// If you want to use this project, mail to ggoggal@gmail.com

#include "StatComponent.h"

FStat& UStatComponent::GetStat(EStat StatType)
{
	FStat* FindStat = Stats.FindByKey(StatType);

	if(FindStat)
	{
		return *FindStat;
	}
	else
	{
		return FStat::StatNone;
	}
}

void UStatComponent::SetStat(const FStat& InStat)
{
	FStat& FindStat = GetStat(InStat.StatType);

	if (FindStat.IsValidStat())
	{
		FindStat = InStat;
	}
	else
	{
		Stats.Add(InStat);
	}
}

void UStatComponent::SetStat(EStat StatType, float Value)
{
	FStat& FindStat = GetStat(StatType);

	if (FindStat.IsValidStat())
	{
		FindStat.Value = Value;
	}
	else
	{
		Stats.Add(FStat(StatType, Value));
	}
}

void UStatComponent::AddStat(EStat StatType, float AddValue)
{
	FStat& FindStat = GetStat(StatType);

	if (FindStat.IsValidStat())
	{
		FindStat.Value += AddValue;
	}
	else
	{
		Stats.Add(FStat(StatType, AddValue));
		LOG(Error, TEXT("UStatComponent::AddStat Used As Not 'Add', 'Set'"));
	}
}