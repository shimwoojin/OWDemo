// If you want to use this project, mail to ggoggal@gmail.com


#include "CoolTimeSkill.h"
#include "BaseCharacter.h"
#include "MontagePlayDataAsset.h"
#include "BaseWeapon.h"

ACoolTimeSkill::ACoolTimeSkill()
{
	CoolTime = 10.f;
	bCoolTime = false;
}

bool ACoolTimeSkill::Activate()
{
	if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetInstigator()))
	{
		if (CanUseSkill() && BaseCharacter->IsSkillable())
		{
			ABaseWeapon* SkillOwner = Cast<ABaseWeapon>(GetOwner());
			FMontagePlayData SkillMontageData = SkillMontage->GetMontagePlayData(0);
			FOnMontageEnded OnMontageEnded;

			OnMontageEnded.BindLambda([this, BaseCharacter, SkillOwner](UAnimMontage* AnimMontage, bool bInterrupted) {

				if (!bInterrupted)
				{
					BaseCharacter->SetState(ECharacterState::Idle);
				}
				
				if (SkillOwner && BaseCharacter->IsState(ECharacterState::Skill) == false)
				{
					SkillOwner->SetActiveSkill(nullptr);
				}

				});

			if (SkillMontageData.AnimMontage && SkillOwner)
			{
				SkillOwner->SetActiveSkill(this);
				BaseCharacter->MontagePlay(SkillMontageData);
				BaseCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded, SkillMontageData.AnimMontage);

				bCoolTime = true;
				SkillCanNotUse();

				GetWorldTimerManager().SetTimer(CoolTimeHandle, FTimerDelegate::CreateLambda([this]() {

					bCoolTime = false;
					SkillCanUse();

					}), CoolTime, false);
			}

			BaseCharacter->SetState(ECharacterState::Skill);
			return true;
		}
	}

	return false;
}

float ACoolTimeSkill::GetCoolTime()
{
	return CoolTime;
}

bool ACoolTimeSkill::CanUseSkill()
{
	return Super::CanUseSkill() && !bCoolTime;
}

float ACoolTimeSkill::GetSkillUsagePercent()
{
	if (CoolTimeHandle.IsValid())
	{
		return GetWorldTimerManager().GetTimerRemaining(CoolTimeHandle) / GetCoolTime();
	}
	else
	{
		return 0.f;
	}
}