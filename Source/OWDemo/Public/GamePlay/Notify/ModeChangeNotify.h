// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ModeChangeNotify.generated.h"

UCLASS()
class OWDEMO_API UModeChangeNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) final;
};
