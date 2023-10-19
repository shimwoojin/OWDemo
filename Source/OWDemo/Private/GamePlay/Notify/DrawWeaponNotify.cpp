// If you want to use this project, mail to ggoggal@gmail.com

#include "DrawWeaponNotify.h"
#include "SwapManagement.h"

void UDrawWeaponNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	SMOOTH_CHECK(MeshComp);

	if (auto* const SwapableCharacter = Cast<ISwapManagement>(MeshComp->GetOwner()))
	{
		SwapableCharacter->Draw();
	}
}