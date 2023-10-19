// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SheathWeaponNotify.generated.h"

UCLASS()
class OWDEMO_API USheathWeaponNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) final;
};
