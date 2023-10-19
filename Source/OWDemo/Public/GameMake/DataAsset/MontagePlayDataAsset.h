// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "Engine/DataAsset.h"
#include "AnimationStructure.h"
#include "MontagePlayDataAsset.generated.h"

UCLASS()
class OWDEMO_API UMontagePlayDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FMontagePlayData GetMontagePlayData(int32 Index);
	FMontagePlayData FindMontagePlayData(const FString& IncludingText);
	TArray<FMontagePlayData> FindAllMontagePlayData(const FString& IncludingText);

private:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = MontagePlayData, meta = (AllowPrivateAccess = true))
		TArray<FMontagePlayData> MontagePlayData;
};
