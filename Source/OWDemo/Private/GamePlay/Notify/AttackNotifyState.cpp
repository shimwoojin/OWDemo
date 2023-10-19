// If you want to use this project, mail to ggoggal@gmail.com


#include "AttackNotifyState.h"
#include "AttackManagement.h"

void UAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	TotalDurationTime = TotalDuration;

	if (auto* const Attacker = Cast<IAttackManagement>(MeshComp->GetOwner()))
	{
		Attacker->StartAttack();
	}
}

void UAttackNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (auto* const Attacker = Cast<IAttackManagement>(MeshComp->GetOwner()))
	{
		Attacker->AttackTick(FrameDeltaTime, TotalDurationTime);
	}
}

void UAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (auto* const Attacker = Cast<IAttackManagement>(MeshComp->GetOwner()))
	{
		Attacker->EndAttack();
	}
}