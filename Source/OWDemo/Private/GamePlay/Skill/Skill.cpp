// If you want to use this project, mail to ggoggal@gmail.com

#include "Skill.h"

ASkill::ASkill()
{
	PrimaryActorTick.bCanEverTick = true;

	SkillCanUse();
}

void ASkill::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ASkill::CanUseSkill()
{
	return bCanUseSkill;
}

bool ASkill::Activate()
{
	LOG(Warning, TEXT("You Don't Override ASkill::Activate()"));
	return false;
}

void ASkill::SkillCanUse()
{
	bCanUseSkill = true;
}

void ASkill::SkillCanNotUse()
{
	bCanUseSkill = false;
}

