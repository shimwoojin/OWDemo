// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "MeleeWeapon.h"
#include "BaseShield.generated.h"

UCLASS()
class OWDEMO_API ABaseShield : public AMeleeWeapon
{
	GENERATED_BODY()
	
public:
	ABaseShield();

public:
	//ActingManagement
	virtual void Dash(bool bPressed) final;
	virtual void OnDashNotify() final;
	virtual void Attack(bool bPressed) final;
	virtual void AttackAssist(bool bPressed) final;

public:
	virtual bool CanArmingAsSubWeapon(ABaseWeapon* const InMainArmingWeapon) final;
	virtual bool CanStandAlone() final { return false; }
	virtual void RegisterMainWeapon(ABaseWeapon* const InMainArmingWeapon);

private:
	ABaseWeapon* MainArmingWeapon;

	float OnDodgeLineTraceDistance;
};
