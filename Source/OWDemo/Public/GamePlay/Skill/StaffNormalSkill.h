// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "CoolTimeSkill.h"
#include "StaffNormalSkill.generated.h"

UCLASS()
class OWDEMO_API AStaffNormalSkill : public ACoolTimeSkill
{
	GENERATED_BODY()
	
public:
	AStaffNormalSkill();

public:
	virtual void OnSkillStartNotify() final;
	virtual void OnSkillEndNotify() final;

private:
	UPROPERTY()
		class USceneComponent* Root;
};
