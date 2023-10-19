// If you want to use this project, mail to ggoggal@gmail.com


#include "CharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (auto* MyPawn = TryGetPawnOwner())
	{
		Speed = MyPawn->GetVelocity().Size2D();
		IsInAir();
	}
}

void UCharacterAnimInstance::SetCrouched(bool bPressed)
{
	bCrouched = bPressed;
}

void UCharacterAnimInstance::SetWeaponMode(EWeaponMode InMode)
{
	WeaponMode = InMode;
}

void UCharacterAnimInstance::SetSoldierWeaponType(ESoldierWeaponType InType)
{
	SoldierWeaponType = InType;
}

void UCharacterAnimInstance::SetNormalWeaponType(ENormalWeaponType InType)
{
	NormalWeaponType = InType;
}

void UCharacterAnimInstance::IsInAir()
{
	if (auto* Pawn = TryGetPawnOwner())
	{
		bIsInAir = Pawn->GetMovementComponent()->IsFalling();
	}
}