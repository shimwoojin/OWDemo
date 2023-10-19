// If you want to use this project, mail to ggoggal@gmail.com

#include "ModeManageComponent.h"
#include "WeaponComponent.h"
#include "BaseWeapon.h"

UModeManageComponent::UModeManageComponent()
{
	CreateDefaultSubobjectAuto(NormalWeaponMode);
	CreateDefaultSubobjectAuto(SoldierWeaponMode);

	NormalWeaponMode->bSoldier = false;
	SoldierWeaponMode->bSoldier = true;
}

void UModeManageComponent::BeginPlay()
{
	Super::BeginPlay();

	NormalWeaponMode->OnSwapWeaponDelegate.AddUObject(this, &ThisClass::OnSwapWeapon);
	SoldierWeaponMode->OnSwapWeaponDelegate.AddUObject(this, &ThisClass::OnSwapWeapon);
}

UWeaponComponent* UModeManageComponent::GetCurrentWeaponComponent()
{
	switch (WeaponMode)
	{
	case EWeaponMode::None:
	{
		break;
	}
	case EWeaponMode::Soldier:
	{
		return SoldierWeaponMode;
	}
	case EWeaponMode::Weapon:
	{
		return NormalWeaponMode;
	}
	}

	return nullptr;
}

void UModeManageComponent::ChangeMode()
{
	ChangeArmingWeapon(false);

	EWeaponMode Before = WeaponMode;
	EWeaponMode After = GetAfterChangeMode(Before);
	WeaponMode = After;

	if (auto* const ModeManagedOwner = Cast<IModeManagement>(GetOwner()))
	{
		ModeManagedOwner->ChangeSkin(WeaponMode);
	}

	ChangeWeaponVisibility(Before, After);
	ChangeArmingWeapon(true);

	OnChangeWeaponMode.Broadcast(After);
	OnSwapWeapon();
}

void UModeManageComponent::ChangeModeNone()
{
	ChangeArmingWeapon(false);

	ChangeWeaponVisibility(WeaponMode, EWeaponMode::None);
	WeaponMode = EWeaponMode::None;

	if (auto* const ModeManagedOwner = Cast<IModeManagement>(GetOwner()))
	{
		ModeManagedOwner->ChangeSkin(WeaponMode);
	}

	OnChangeWeaponMode.Broadcast(WeaponMode);
	OnSwapWeapon();
}

EWeaponMode UModeManageComponent::GetAfterChangeMode(EWeaponMode Before)
{
	EWeaponMode ResultWeaponMode;

	if (Before == EWeaponMode::None)
	{
		ResultWeaponMode = EWeaponMode::Soldier;
	}
	else
	{
		(Before == EWeaponMode::Soldier) ? (ResultWeaponMode = EWeaponMode::Weapon) : (ResultWeaponMode = EWeaponMode::Soldier);
	}

	return ResultWeaponMode;
}

bool UModeManageComponent::SwapWeapon(int32 Slot)
{
	auto* const CurrentWeaponMode = GetCurrentWeaponComponent();
	
	if (!CurrentWeaponMode)
	{
		return false;
	}

	return CurrentWeaponMode->SwapWeapon(Slot);
}

void UModeManageComponent::OnSwapWeapon()
{
	auto* const CurrentWeaponMode = GetCurrentWeaponComponent();
	
	ESoldierWeaponType SoldierWeaponType = ESoldierWeaponType::None;
	ENormalWeaponType NormalWeaponType = ENormalWeaponType::None;

	if (CurrentWeaponMode)
	{
		CurrentWeaponMode->GetArmingWeaponType(SoldierWeaponType, NormalWeaponType);
	}

	OnChangeSoldierWeaponType.Broadcast(SoldierWeaponType);
	OnChangeNormalWeaponType.Broadcast(NormalWeaponType);
}

void UModeManageComponent::InsertWeaponToSlot(EWeaponMode InWeaponMode, ABaseWeapon* const WantForInsert, int32 Slot)
{
	switch (InWeaponMode)
	{
	case EWeaponMode::None:
	{
		return;
		break;
	}
	case EWeaponMode::Soldier:
	{
		SoldierWeaponMode->InsertWeaponToSlot(WantForInsert, Slot);
		
		if (WeaponMode != EWeaponMode::Soldier)
		{
			WantForInsert->Hide();
		}

		break;
	}
	case EWeaponMode::Weapon:
	{
		NormalWeaponMode->InsertWeaponToSlot(WantForInsert, Slot);

		if (WeaponMode != EWeaponMode::Weapon)
		{
			WantForInsert->Hide();
		}

		break;
	}
	}
}

void UModeManageComponent::DeleteWeaponToSlot(EWeaponMode InWeaponMode, int32 Slot)
{
	switch (InWeaponMode)
	{
	case EWeaponMode::None:
	{
		return;
		break;
	}
	case EWeaponMode::Soldier:
	{
		SoldierWeaponMode->DeleteWeaponToSlot(Slot);
		break;
	}
	case EWeaponMode::Weapon:
	{
		NormalWeaponMode->DeleteWeaponToSlot(Slot);
		break;
	}
	}
}

void UModeManageComponent::DeleteWeaponToSlot(EWeaponMode InWeaponMode, class ABaseWeapon* WantToDelete)
{
	switch (InWeaponMode)
	{
	case EWeaponMode::None:
	{
		return;
		break;
	}
	case EWeaponMode::Soldier:
	{
		SoldierWeaponMode->DeleteWeaponToSlot(WantToDelete);
		break;
	}
	case EWeaponMode::Weapon:
	{
		NormalWeaponMode->DeleteWeaponToSlot(WantToDelete);
		break;
	}
	}
}

void UModeManageComponent::ChangeArmingWeapon(bool bEquip)
{
	if (auto* const CurrentWeaponMode = GetCurrentWeaponComponent())
	{
		if (CurrentWeaponMode->IsArmingWeaponNow())
		{
			auto* EquipedOrUnequiped = CurrentWeaponMode->GetArmingWeapon();

			if (auto* const Player = Cast<ABaseCharacter>(GetOwner()))
			{
				EquipedOrUnequiped->AttachTo(Player, bEquip);
			}
		}
	}
}

void UModeManageComponent::ChangeWeaponVisibility(EWeaponMode Before, EWeaponMode After)
{
	switch (Before)
	{
	case EWeaponMode::Soldier:
	{
		auto const Weapons = SoldierWeaponMode->GetWeapons();

		for (auto* const Weapon : Weapons)
		{
			if (IsValid(Weapon))
			{
				Weapon->Hide();
			}
		}

		break;
	}
	case EWeaponMode::Weapon:
	{
		auto const Weapons = NormalWeaponMode->GetWeapons();

		for (auto* const Weapon : Weapons)
		{
			if (IsValid(Weapon))
			{
				Weapon->Hide();
			}
		}

		break;
	}
	}

	switch (After)
	{
	case EWeaponMode::Soldier:
	{
		auto const Weapons = SoldierWeaponMode->GetWeapons();

		for (auto* const Weapon : Weapons)
		{
			if (IsValid(Weapon))
			{
				Weapon->Show();
			}
		}

		break;
	}
	case EWeaponMode::Weapon:
	{
		auto const Weapons = NormalWeaponMode->GetWeapons();

		for (auto* const Weapon : Weapons)
		{
			if (IsValid(Weapon))
			{
				Weapon->Show();
			}
		}

		break;
	}
	}
}
