// If you want to use this project, mail to ggoggal@gmail.com

#include "MontagePlayDataAsset.h"

FMontagePlayData UMontagePlayDataAsset::GetMontagePlayData(int32 Index)
{
	/*if (Index < 0 || Index > MontagePlayData.Num())
	{
		check(0 && "Invalid Index");
	}*/

	SMOOTH_CHECK(Index >= 0 || Index < MontagePlayData.Num(), FMontagePlayData());

	return MontagePlayData[Index];
}

FMontagePlayData UMontagePlayDataAsset::FindMontagePlayData(const FString& IncludingText)
{
	if (IncludingText.IsEmpty())
	{
		LOG(Warning, TEXT("Fill Text To Find Montage Data"));
		return FMontagePlayData();
	}

	for (auto& MontageData : MontagePlayData)
	{
		FString MontageName = MontageData.AnimMontage->GetFName().ToString();
		
		if (MontageName.Find(IncludingText) != INDEX_NONE)
		{
			return MontageData;
		}
	}

	LOG(Log, TEXT("Not Found MontageData"));
	return FMontagePlayData();
}

TArray<FMontagePlayData> UMontagePlayDataAsset::FindAllMontagePlayData(const FString& IncludingText)
{
	TArray<FMontagePlayData> ReturnMontages;

	if (IncludingText.IsEmpty())
	{
		LOG(Warning, TEXT("Fill Text To Find Montage Data"));
		return ReturnMontages;
	}

	for (auto& MontageData : MontagePlayData)
	{
		FString MontageName = MontageData.AnimMontage->GetFName().ToString();

		if (MontageName.Find(IncludingText) != INDEX_NONE)
		{
			ReturnMontages.Add(MontageData);
		}
	}

	return ReturnMontages;
}
