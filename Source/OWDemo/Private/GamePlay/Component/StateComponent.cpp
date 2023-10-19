// If you want to use this project, mail to ggoggal@gmail.com


#include "StateComponent.h"
#include "BaseEnemy.h"
#include "BasePlayer.h"
#include "ModeManageComponent.h"
#include "WeaponComponent.h"

UStateComponent::UStateComponent()
{
    State = ECharacterState::Idle;
}

void UStateComponent::SetState(ECharacterState CharacterState)
{
    State = CharacterState;
}

ECharacterState UStateComponent::GetState()
{
    return State;
}

bool UStateComponent::IsState(ECharacterState CharacterState)
{
    return State == CharacterState;
}

bool UStateComponent::IsChangeable(ECharacterState CharacterState)
{
    return true;
}

bool UStateComponent::IsArmed()
{
    bool bResult = false;

    if (ABasePlayer* const BasePlayer = Cast<ABasePlayer>(GetOwner()))
    {
        bResult = BasePlayer->GetModeManager()->GetCurrentWeaponComponent()->IsArmingWeaponNow();
    }
    else if (ABaseEnemy* const BaseEnemy = Cast<ABaseEnemy>(GetOwner()))
    {
        bResult = BaseEnemy->GetWeaponManager()->IsArmingWeaponNow();
    }

    return bResult;
}

bool UStateComponent::IsDashable()
{
    return !(
        IsState(ECharacterState::Dash) ||
        IsState(ECharacterState::Death) ||
        IsState(ECharacterState::Swap) ||
        IsZoomOn()
        );
}

bool UStateComponent::IsSkillable()
{
    return !(
        IsState(ECharacterState::Dash) ||
        IsState(ECharacterState::Death) ||
        IsState(ECharacterState::Attack) ||
        IsState(ECharacterState::Swap) ||
        IsZoomOn()
        );
}

bool UStateComponent::IsAttackable()
{
    return !(
        IsState(ECharacterState::Dash) ||
        IsState(ECharacterState::Death) ||
        IsState(ECharacterState::Skill) ||
        IsState(ECharacterState::Swap)
        );
}

bool UStateComponent::IsAttackAssistable()
{
    return !(
        IsState(ECharacterState::Dash) ||
        IsState(ECharacterState::Death) ||
        IsState(ECharacterState::Attack) ||
        IsState(ECharacterState::Skill) ||
        IsState(ECharacterState::Swap) ||
        IsState(ECharacterState::Reload) ||
        IsState(ECharacterState::ModeChange) ||
        IsZoomOn()
        );
}

bool UStateComponent::IsReloadable()
{
    return !(
        IsState(ECharacterState::Dash) ||
        IsState(ECharacterState::Death) ||
        IsState(ECharacterState::Reload) ||
        IsState(ECharacterState::Swap) ||
        IsZoomOn()
        );
}

bool UStateComponent::IsSwapable()
{
    return !(
        IsState(ECharacterState::Dash) ||
        IsState(ECharacterState::Death) ||
        IsState(ECharacterState::Attack) ||
        IsState(ECharacterState::Skill) ||
        IsState(ECharacterState::Swap) ||
        IsZoomOn()
        );
}

bool UStateComponent::IsModeChangeable()
{
    return !(
        IsState(ECharacterState::Dash) ||
        IsState(ECharacterState::Death) ||
        IsState(ECharacterState::Attack) ||
        IsState(ECharacterState::Skill) ||
        IsState(ECharacterState::Swap) ||
        IsState(ECharacterState::Reload) ||
        IsState(ECharacterState::ModeChange) ||
        IsZoomOn()
        );
}

bool UStateComponent::IsHitMontagePlayable()
{
    return !(
        IsState(ECharacterState::Dash) ||
        IsState(ECharacterState::Death) ||
        IsState(ECharacterState::Swap) ||
        IsState(ECharacterState::ModeChange)
        );
}

void UStateComponent::SetGaitState(EGaitState InGaitState)
{
    GaitState = InGaitState;
}

EGaitState UStateComponent::GetGaitState()
{
    return GaitState;
}

bool UStateComponent::IsSameGaitState(EGaitState InGaitState)
{
    return GaitState == InGaitState;
}
