// If you want to use this project, mail to ggoggal@gmail.com

#include "StaffNormalSkill.h"
#include "BaseStaff.h"

AStaffNormalSkill::AStaffNormalSkill()
{
	CreateDefaultSubobjectAuto(Root);
	SetRootComponent(Root);

	SetCoolTime(2.5f);

#pragma region AssetSetup
	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> SKILL_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/Skill/DA_StaffNormalSkillMontage.DA_StaffNormalSkillMontage'"));
	if (SKILL_MONTAGE.Succeeded())
	{
		SkillMontage = SKILL_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> SKILL_TEXTURE
	(TEXT("Texture2D'/Game/Assets/Textures/Skill/EnergyBall.EnergyBall'"));
	if (SKILL_TEXTURE.Succeeded())
	{
		SkillTexture = SKILL_TEXTURE.Object;
	}
#pragma endregion
}

void AStaffNormalSkill::OnSkillStartNotify()
{
	Super::OnSkillStartNotify();
}

void AStaffNormalSkill::OnSkillEndNotify()
{
	Super::OnSkillEndNotify();

	if (auto* const Staff = Cast<ABaseStaff>(GetOwner()))
	{
		Staff->ChangeElementalType();
	}
}
