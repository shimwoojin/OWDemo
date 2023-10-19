// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "BaseEnemy.h"
#include "Griffon.generated.h"

UENUM()
enum class EGriffonMode : uint8
{
	Ground,
	Fly,
	StationaryFly,
	Max
};

UENUM()
enum class EGriffonState : uint8
{
	Idle,
	Attack,
	Hitted,
	Falling,
	Landing
};

UENUM()
enum class EGriffonSpeed : uint8
{
	Zero,
	Normal,
	Fast,
	Max
};

UCLASS()
class OWDEMO_API AGriffon : public ABaseEnemy
{
	GENERATED_BODY()
	
public:
	AGriffon();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION()
		void OnFlyAttackOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	//ActingManagement
	virtual void Attack(bool bPressed) final;

	//AttackManagement
	virtual void StartAttack() final;
	virtual void AttackTick(float DeltaTime, float TotalDuration) final;
	virtual void EndAttack() final;

public:
	EGriffonMode GetGriffonMode(){ return GriffonMode; }
	EGriffonState GetGriffonState() { return GriffonState; }
	EGriffonSpeed GetGriffonSpeed() { return GriffonSpeed; }

	void SetGriffonMode(EGriffonMode InGriffonMode);
	void SetGriffonState(EGriffonState InGriffonState);
	void SetGriffonSpeed(EGriffonSpeed InGriffonSpeed);

private:
	void OnTickDecideHeight(float DeltaSeconds);

private:
	TMap<EGriffonSpeed, int> GriffonSpeedMap;

private:
	EGriffonMode GriffonMode;
	EGriffonState GriffonState;
	EGriffonSpeed GriffonSpeed;

	float FlyHeight;
	float StationaryFlyHeight;

	UPROPERTY(EditDefaultsOnly)
		class UMontagePlayDataAsset* GriffonMontages;

	FVector FlyAttackStartLocation;
	FVector FlyAttackAimLocation;
	float TargetBackDistance;
	bool bTargetLocationSet = false;
	float AccumulatedAttackTickTime = 0.f;
	float MontagePlayRate = 1.f;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = true))
		class UCapsuleComponent* FlyAttackCapsule;

	TSet<class ABasePlayer*> OneTimeDamage;

	float GroundAttackDamage = 20.f;
	float FlyAttackDamage = 50.f;
	float StationaryFlyAttackDamage = 30.f;
};