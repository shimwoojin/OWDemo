// If you want to use this project, mail to ggoggal@gmail.com

#include "ModeChangeNoneNotify.h"
#include "BasePlayer.h"

void UModeChangeNoneNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	SMOOTH_CHECK(MeshComp);

	if (auto* const BasePlayer = Cast<ABasePlayer>(MeshComp->GetOwner()))
	{
		BasePlayer->OnNotifyModeChangeNone();
	}
}