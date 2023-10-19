#pragma once
#include "OWDemo.h"
#include "UObject/Object.h"
#include "Animation/AnimInstance.h"
#include "AnimationStructure.generated.h"

USTRUCT(BlueprintType)
struct FMontagePlayData
{
	GENERATED_BODY()

public:
	FMontagePlayData() 
		: AnimMontage(nullptr),
		PlayRate(1.f), 
		ReturnType(EMontagePlayReturnType::MontageLength), 
		StartAt(0.f), 
		StopAllMontages(true)
	{}

public:
	UPROPERTY(EditDefaultsOnly, Category = MontagePlay)
		UAnimMontage* AnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = MontagePlay)
		float PlayRate;

	UPROPERTY(EditDefaultsOnly, Category = MontagePlay)
		EMontagePlayReturnType ReturnType;

	UPROPERTY(EditDefaultsOnly, Category = MontagePlay)
		float StartAt;

	UPROPERTY(EditDefaultsOnly, Category = MontagePlay)
		bool StopAllMontages;
};

