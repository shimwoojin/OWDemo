// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "CoolTimeSkill.generated.h"

UCLASS(Abstract)
class OWDEMO_API ACoolTimeSkill : public ASkill
{
	GENERATED_BODY()
	
public:
	ACoolTimeSkill();

public:
	virtual bool Activate() override;

	UFUNCTION(BlueprintCallable)
		float GetCoolTime();

	UFUNCTION(BlueprintCallable)
		void SetCoolTime(float InCoolTime) { CoolTime = InCoolTime; }

	virtual bool CanUseSkill() override;

	virtual float GetSkillUsagePercent() final;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CoolTime, meta = (AllowPrivateAccess = true))
		float CoolTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CoolTime, meta = (AllowPrivateAccess = true))
		bool bCoolTime;

	FTimerHandle CoolTimeHandle;
};
