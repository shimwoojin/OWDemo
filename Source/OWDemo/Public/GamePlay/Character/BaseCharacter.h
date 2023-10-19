//If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "GameFramework/Character.h"
#include "ActingManagement.h"
#include "StateManagement.h"
#include "GaitStateManagement.h"
#include "StatManagement.h"
#include "CharacterManagement.h"
#include "SwapManagement.h"
#include "AttackManagement.h"
#include "SkillActivateManagement.h"
#include "BaseCharacter.generated.h"

DECLARE_DELEGATE(FDrawDelegate);
DECLARE_DELEGATE(FSheathDelegate);

namespace WalkSpeed
{
	enum Type
	{
		Idle = 0,
		Walk = 300,
		Run = 600,
		Sprint = 900
	};
}

UCLASS(Abstract)
class OWDEMO_API ABaseCharacter
	: 
	public ACharacter,
	public IActingManagement,
	public IStateManagement,
	public IStatManagement,
	public ICharacterManagement,
	public ISwapManagement,
	public IAttackManagement,
	public IGaitStateManagement,
	public ISkillActivateManagement
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) final;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) final;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	void MontagePlay(const FMontagePlayData& MontagePlayData);
	virtual float SetMaxHP(float MaxHP, bool bEffectToCurrentHP);
	virtual float AddCurrentHP(float AddHP);

	void PickUpItem_MontagePlay();
	virtual FVector GetRightHandLocation();

public:

	//CharacterManagement
	virtual void Sprint(bool bPressed) override;
	virtual void Run() override;
	virtual void Walk() override;
	virtual void Swap(int32 Number) override;

	//ActingManagement
	virtual void Dash(bool bPressed) override;
	virtual void Attack(bool bPressed) override;
	virtual void AttackAssist(bool bPressed) override;
	virtual void ActivateSkill(int32 Number, bool bPressed) override;

protected:
	virtual void Dodge();

public:

	//SwapManagement
	virtual void Draw() final;
	virtual void Sheath() final;

public:

	//StateManagement
	virtual void SetState(ECharacterState CharacterState) final;
	virtual ECharacterState GetState() final;
	virtual bool IsState(ECharacterState CharacterState) final;
	virtual bool IsChangeable(ECharacterState CharacterState) final;
	virtual bool IsArmed() final;
	virtual bool IsDashable() final;
	virtual bool IsSkillable() final;
	virtual bool IsAttackable() final;
	virtual bool IsAttackAssistable() final;
	virtual bool IsReloadable() final;
	virtual bool IsSwapable() final;
	virtual bool IsModeChangeable() final;
	virtual bool IsHitMontagePlayable() final;

public:

	//GaitStateManagement
	virtual void SetGaitState(EGaitState InGaitState) final;
	virtual EGaitState GetGaitState() final;
	virtual bool IsSameGaitState(EGaitState InGaitState) final;

public:

	//StatManagement
	virtual FStat& GetStat(EStat StatType) final;
	virtual void SetStat(const FStat& Stat) final;
	virtual void SetStat(EStat StatType, float Value) final;
	virtual void AddStat(EStat StatType, float AddValue) final;

private:
	UFUNCTION()
		void OnSprint(float Alpha);

	EHittedDirection CalculateHitFromDirection(AActor* DamageCauser, AController* EventInstigator);
		
public:
	FDrawDelegate OnDraw;
	FSheathDelegate OnSheath;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
		class UStatComponent* Stat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = true))
		class UStateComponent* State;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sprint, meta = (AllowPrivateAccess = true))
		class UTimelineComponent* SprintTimeline;

	UPROPERTY(EditDefaultsOnly, Category = Sprint)
		class UCurveFloat* SprintCurve;

	float SprintConvertTime;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
		class UMontagePlayDataAsset* DodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Hitted")
		class UMontagePlayDataAsset* HittedMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Death")
		class UMontagePlayDataAsset* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
		class UMontagePlayDataAsset* PickUpMontage;
};
