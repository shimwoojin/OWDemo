// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DashNotify.generated.h"

UCLASS()
class OWDEMO_API UDashNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) final;
};
