// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "OWEnums.h"
#include "CharacterBaseComponent.h"
#include "StateManagement.h"
#include "GaitStateManagement.h"
#include "StateComponent.generated.h"

UCLASS()
class OWDEMO_API UStateComponent :
	public UCharacterBaseComponent,
	public IStateManagement,
	public IGaitStateManagement
{
	GENERATED_BODY()

public:
	UStateComponent();
	
public:

	//State
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
	
	//GaitState
	virtual void SetGaitState(EGaitState InGaitState) final;
	virtual EGaitState GetGaitState() final;
	virtual bool IsSameGaitState(EGaitState InGaitState) final;

public:
	void SetZoomAlpha(float Alpha) { ZoomAlpha = Alpha; }
	bool IsZoomOn() { return ZoomAlpha > KINDA_SMALL_NUMBER; }

private:
	ECharacterState State;
	EGaitState GaitState;
	float ZoomAlpha = 0.f;
};
