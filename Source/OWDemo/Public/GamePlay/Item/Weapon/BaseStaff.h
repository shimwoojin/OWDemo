// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "RangedWeapon.h"
#include "OWEnums.h"
#include "BaseStaff.generated.h"

struct FElementalBallProperty
{
	float MaxSize;
	float MaxChargeTime;
	float Speed;
	float MaxSpeed;
	float DefaultSpeed;
};

UCLASS()
class OWDEMO_API ABaseStaff : public ARangedWeapon
{
	GENERATED_BODY()
	
public:
	ABaseStaff();
	virtual void PostInitializeComponents() final;

public:
	//ActingManagement
	virtual void Dash(bool bPressed) final;
	virtual void Attack(bool bPressed) final;
	virtual void AttackAssist(bool bPressed) final;

public:
	void ChangeElementalType();

private:
	void ReserveChargeAttack();
	void DoNormalAttack();

private:
	virtual void StartAttack() final;
	virtual void EndAttack() final;

	void SpawnElementalBall();

	UFUNCTION()
		void ChargeElementalBall(float Alpha);

private:
	bool bNormalAttack = false;
	bool bChargeAttack = false;
	FTimerHandle ChargeAttackHandle;
	float ChargeAttackDelay;

	FMontagePlayData ChargeAttackMontage;

	EElementalType ElementalTypeNow = EElementalType::Thunder;
	TMap<EElementalType, UClass*> ElementalBallClassMap;
	UClass* SelectedElementalBallClass;
	class AElementalBall* ElementalBall;

	FElementalBallProperty ElementalBallProperty;

	UPROPERTY(VisibleAnywhere, Category = Charge)
		class UTimelineComponent* ChargeTimeline;

	UPROPERTY(EditDefaultsOnly)
		class UCurveFloat* ChargeCurve;
};
