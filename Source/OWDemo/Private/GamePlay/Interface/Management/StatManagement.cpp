// If you want to use this project, mail to ggoggal@gmail.com

#include "StatManagement.h"

FStat& IStatManagement::GetStat(EStat StatType)
{
	LOG(Warning, TEXT("IStatManagement::GetStat() Not Implemented"));
	return FStat::StatNone;
}

void IStatManagement::SetStat(const FStat& Stat)
{
	LOG(Warning, TEXT("IStatManagement::SetStat() Not Implemented"));
}

void IStatManagement::SetStat(EStat StatType, float Value)
{
	LOG(Warning, TEXT("IStatManagement::SetStat() Not Implemented"));
}

void IStatManagement::AddStat(EStat StatType, float AddValue)
{
	LOG(Warning, TEXT("IStatManagement::AddStat() Not Implemented"));
}