// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "MeleeWeapon.h"
#include "BaseSword.generated.h"

UCLASS()
class OWDEMO_API ABaseSword : public AMeleeWeapon
{
	GENERATED_BODY()
	
public:
	ABaseSword();
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) final;

public:

	//ActingManagement
	virtual void Dash(bool bPressed) final;
	virtual void OnDashNotify() final;
	virtual void Attack(bool bPressed) final;
	virtual void AttackAssist(bool bPressed) final;

	//AttackManagement
	virtual void StartAttack() final;
	virtual void EndAttack() final;

	virtual float GuardDamageAsPercent() final;

private:
	void ReserveComboAttack();
	void CancelComboAttack();
	void DoNormalAttack();
	void DoLowAttack();
	void DoJumpAttack();

	void GuardStart();
	void GuardEnd();

private:
	FTimerHandle ComboAttackTimer;
	float ComboAttackDelay;
	bool bComboOn = false;
	bool bNormalAttack = false;
	bool bLowAttack = false;
	bool bJumpAttack = false;

	TSet<AActor*> OneTimeDamagedActors;

	float DodgeDistance;
	float OnDodgeLineTraceDistance;

	FMontagePlayData GuardStartMontage;
};
