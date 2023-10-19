#pragma once

#include "OWDemo.h"
#include "WeaponStructure.generated.h"

USTRUCT(BlueprintType)
struct FZoomProperty
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = Zoom)
		bool bZoomable;

	UPROPERTY(EditDefaultsOnly, Category = Zoom)
		float BaseFieldOfView;

	UPROPERTY(EditDefaultsOnly, Category = Zoom)
		float ZoomRate;
	
	UPROPERTY(EditDefaultsOnly, Category = Zoom)
		float ZoomSpeed;
};
