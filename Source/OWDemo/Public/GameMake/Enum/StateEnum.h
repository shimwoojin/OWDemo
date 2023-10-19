#pragma once

#include "OWDemo.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle,
	Attack,
	AttackAssist,
	Dash,
	Skill,
	Reload,
	Swap,
	Hitted,
	ModeChange,
	Death
};

UENUM()
enum class EGaitState : uint8
{
	Stand,
	Walk,
	Run,
	Sprint,
	Jump,
	Sit
};

UENUM()
enum class EHittedDirection : uint8
{
	Any,
	Forward,
	Backward,
	Left,
	Right
};