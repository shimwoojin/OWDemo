// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "RangedWeapon.h"
#include "BaseBow.generated.h"

UCLASS()
class OWDEMO_API ABaseBow : public ARangedWeapon
{
	GENERATED_BODY()
	
public:
	ABaseBow();

	virtual void PostInitializeComponents() final;

public:
	//ActingManagement
	virtual void Dash(bool bPressed) final;
	virtual void Attack(bool bPressed) final;
	virtual void AttackAssist(bool bPressed) final;

private:
	void DrawArrow();
	void ShotArrow();

	virtual void StartAttack() final;
	virtual void AttackTick(float DeltaTime, float TotalDuration) final;
	virtual void EndAttack() final;

	UFUNCTION()
		void ChargeArrowDistance(float Alpha);

private:
	UPROPERTY(VisibleAnywhere, Category = ShotGuide)
		class UArrowComponent* ShotGuider;

	UPROPERTY(VisibleAnywhere, Category = ChargeAttack)
		class UTimelineComponent* ChargeTimeline;

	UPROPERTY(EditDefaultsOnly, Category = ChargeAttack)
		class UCurveFloat* ChargeCurve;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AArrow> ArrowClass;

	class AArrow* BeforeShotedArrow;

	FMontagePlayData DrawAndShotMontage;

	float MaxArrowSpeed;
	float ChargedArrowSpeed;
	float MaxZoomTime;
};
