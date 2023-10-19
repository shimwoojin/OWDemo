// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "OWEnums.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

UCLASS()
class OWDEMO_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	virtual void SetCrouched(bool bPressed);
	void SetWeaponMode(EWeaponMode InMode);
	void SetSoldierWeaponType(ESoldierWeaponType InType);
	void SetNormalWeaponType(ENormalWeaponType InType);

private:
	void IsInAir();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mode, meta = (AllowPrivateAccess = true))
		EWeaponMode WeaponMode = EWeaponMode::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mode, meta = (AllowPrivateAccess = true))
		ESoldierWeaponType SoldierWeaponType = ESoldierWeaponType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mode, meta = (AllowPrivateAccess = true))
		ENormalWeaponType NormalWeaponType = ENormalWeaponType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		bool bCrouched;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		float Speed;
};
