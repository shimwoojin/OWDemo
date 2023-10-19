// If you want to use this project, mail to ggoggal@gmail.com

#include "ModeChangeNotify.h"
#include "BasePlayer.h"

void UModeChangeNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	SMOOTH_CHECK(MeshComp);

	if (auto* const BasePlayer = Cast<ABasePlayer>(MeshComp->GetOwner()))
	{
		BasePlayer->OnNotifyModeChange();
	}
}