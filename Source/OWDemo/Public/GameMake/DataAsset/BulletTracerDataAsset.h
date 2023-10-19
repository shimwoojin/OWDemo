// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "Engine/DataAsset.h"
#include "WeaponEnum.h"
#include "BulletTracerDataAsset.generated.h"

UCLASS()
class OWDEMO_API UBulletTracerDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UBulletTracerDataAsset();

	class UParticleSystem* GetTracerParticle(ESoldierWeaponType WeaponType);

private:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = BulletTracer, meta = (AllowPrivateAccess = true))
		TMap<ESoldierWeaponType, class UParticleSystem*> TracerParticleMap;
};
