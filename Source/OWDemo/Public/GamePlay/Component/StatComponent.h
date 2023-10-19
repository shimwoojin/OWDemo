// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "CharacterBaseComponent.h"
#include "StatManagement.h"
#include "StatComponent.generated.h"

UCLASS()
class OWDEMO_API UStatComponent : 
	public UCharacterBaseComponent,
	public IStatManagement
{
	GENERATED_BODY()
	
public:
	virtual FStat& GetStat(EStat StatType) final;
	virtual void SetStat(const FStat& Stat) final;
	virtual void SetStat(EStat StatType, float Value) final;
	virtual void AddStat(EStat StatType, float AddValue) final;

private:
	UPROPERTY()
		TArray<FStat> Stats;
};
