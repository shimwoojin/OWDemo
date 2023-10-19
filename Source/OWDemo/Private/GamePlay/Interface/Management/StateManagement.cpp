// If you want to use this project, mail to ggoggal@gmail.com

#include "StateManagement.h"
//#include "BaseCharacter.h"

void IStateManagement::SetState(ECharacterState State)
{
    unimplemented();
}

ECharacterState IStateManagement::GetState()
{
    unimplemented();
    return ECharacterState();
}

bool IStateManagement::IsState(ECharacterState State)
{
    return false;
}

bool IStateManagement::IsChangeable(ECharacterState State)
{
    return false;
}

bool IStateManagement::IsArmed()
{
    return false;
}

bool IStateManagement::IsDashable()
{
    return false;
}

bool IStateManagement::IsSkillable()
{
    return false;
}

bool IStateManagement::IsAttackable()
{
    return false;
}

bool IStateManagement::IsAttackAssistable()
{
    return false;
}

bool IStateManagement::IsReloadable()
{
    return false;
}

bool IStateManagement::IsSwapable()
{
    return false;
}

bool IStateManagement::IsModeChangeable()
{
    return false;
}

bool IStateManagement::IsHitMontagePlayable()
{
    return false;
}