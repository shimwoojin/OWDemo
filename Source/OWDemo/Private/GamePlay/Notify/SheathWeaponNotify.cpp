// If you want to use this project, mail to ggoggal@gmail.com


#include "SheathWeaponNotify.h"
#include "SwapManagement.h"

void USheathWeaponNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	SMOOTH_CHECK(MeshComp);

	if (auto* const SwapableCharacter = Cast<ISwapManagement>(MeshComp->GetOwner()))
	{
		SwapableCharacter->Sheath();
	}
}