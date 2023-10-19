// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "ManagementBase.h"
#include "GameMake.h"
#include "StateManagement.generated.h"

UINTERFACE(MinimalAPI)
class UStateManagement : public UManagementBase
{
	GENERATED_BODY()
};

class OWDEMO_API IStateManagement : public IManagementBase
{
	GENERATED_BODY()

public:

	UFUNCTION(Category = State)
		virtual void SetState(enum ECharacterState State);

	UFUNCTION(Category = State)
		virtual ECharacterState GetState();

	UFUNCTION(Category = State)
		virtual bool IsState(ECharacterState State);

	UFUNCTION(Category = State)
		virtual bool IsChangeable(ECharacterState State);

	UFUNCTION(Category = State)
		virtual bool IsArmed();

	UFUNCTION(Category = State)
		virtual bool IsDashable();

	UFUNCTION(Category = State)
		virtual bool IsSkillable();

	UFUNCTION(Category = State)
		virtual bool IsAttackable();

	UFUNCTION(Category = State)
		virtual bool IsAttackAssistable();

	UFUNCTION(Category = State)
		virtual bool IsReloadable();

	UFUNCTION(Category = State)
		virtual bool IsSwapable();

	UFUNCTION(Category = State)
		virtual bool IsModeChangeable();

	UFUNCTION(Category = State)
		virtual bool IsHitMontagePlayable();
};
