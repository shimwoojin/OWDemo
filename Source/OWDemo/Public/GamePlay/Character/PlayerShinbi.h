// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "BasePlayer.h"
#include "PlayerShinbi.generated.h"

UCLASS()
class OWDEMO_API APlayerShinbi : public ABasePlayer
{
	GENERATED_BODY()

public:
	APlayerShinbi();

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void ChangeSkin(EWeaponMode WeaponMode) final;

public:
	UPROPERTY(EditDefaultsOnly)
		class USkeletalMesh* ShinbiBasicSkin;

	UPROPERTY(EditDefaultsOnly)
		class USkeletalMesh* ShinbiSoldierModeSkin;

	UPROPERTY(EditDefaultsOnly)
		class USkeletalMesh* ShinbiWeaponModeSkin;
};
