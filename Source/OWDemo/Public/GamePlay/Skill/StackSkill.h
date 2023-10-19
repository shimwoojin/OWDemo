// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "StackSkill.generated.h"

UCLASS(Abstract)
class OWDEMO_API AStackSkill : public ASkill
{
	GENERATED_BODY()
	
public:
	AStackSkill();

public:

	virtual bool Activate() override;

	virtual bool CanUseSkill() override;

private:
	bool IsStackFull();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stack, meta = (AllowPrivateAccess = true))
		float Stack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stack, meta = (AllowPrivateAccess = true))
		float MaxStack;
};
