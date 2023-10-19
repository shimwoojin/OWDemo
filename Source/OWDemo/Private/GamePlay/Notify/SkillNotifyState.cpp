// If you want to use this project, mail to ggoggal@gmail.com


#include "SkillNotifyState.h"
#include "SkillActivateManagement.h"

void USkillNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (auto* const SkillActivator = Cast<ISkillActivateManagement>(MeshComp->GetOwner()))
	{
		SkillActivator->OnSkillStartNotify();
	}
}

void USkillNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (auto* const SkillActivator = Cast<ISkillActivateManagement>(MeshComp->GetOwner()))
	{
		SkillActivator->OnSkillEndNotify();
	}
}