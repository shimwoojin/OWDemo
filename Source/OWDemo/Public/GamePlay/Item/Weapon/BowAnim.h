// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "Animation/AnimInstance.h"
#include "BowAnim.generated.h"

UCLASS()
class OWDEMO_API UBowAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	void SetArrowLineLocation(FVector InLocation);
	void SetArrowLineAlpha(float InAlpha);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector ArrowLineLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ArrowLineAlpha;
};
