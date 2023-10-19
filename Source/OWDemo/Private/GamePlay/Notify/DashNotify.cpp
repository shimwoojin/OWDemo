// If you want to use this project, mail to ggoggal@gmail.com

#include "DashNotify.h"
#include "ActingManagement.h"

void UDashNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	SMOOTH_CHECK(MeshComp);

	if (auto* const DashableCharacter = Cast<IActingManagement>(MeshComp->GetOwner()))
	{
		DashableCharacter->OnDashNotify();
	}
}
