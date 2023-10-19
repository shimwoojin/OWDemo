// If you want to use this project, mail to ggoggal@gmail.com

#include "BaseShield.h"
#include "BaseSword.h"
#include "BaseCharacter.h"
#include "Components/CapsuleComponent.h"

ABaseShield::ABaseShield()
{
#pragma region AssetSetup
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SWORD
	(TEXT("SkeletalMesh'/Game/Assets/Weapons/Weapon_Pack/Skeletal_Mesh/SK_Shield.SK_Shield'"));
	if (SWORD.Succeeded())
	{
		GetSkeletalMesh()->SetSkeletalMesh(SWORD.Object);
		GetSkeletalMesh()->SetRelativeScale3D(FVector(.8f, .8f, .8f));
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> DRAW_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_BowDrawMontage.DA_BowDrawMontage'"));
	if (DRAW_MONTAGE.Succeeded())
	{
		DrawMontage = DRAW_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> SHEATH_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_BowSheathMontage.DA_BowSheathMontage'"));
	if (SHEATH_MONTAGE.Succeeded())
	{
		SheathMontage = SHEATH_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> ATTACK_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_ShieldAttackMontage.DA_ShieldAttackMontage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> ATTACKASSIST_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_ShieldAttakAssistMontage.DA_ShieldAttakAssistMontage'"));
	if (ATTACKASSIST_MONTAGE.Succeeded())
	{
		AttackAssistMontage = ATTACKASSIST_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> DODGE_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_ShieldDodgeMontage.DA_ShieldDodgeMontage'"));
	if (DODGE_MONTAGE.Succeeded())
	{
		DodgeMontage = DODGE_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> SHIELD_TEXTURE
	(TEXT("Texture2D'/Game/Assets/Textures/Weapon/Shield.Shield'"));
	if (SHIELD_TEXTURE.Succeeded())
	{
		ItemTexture = SHIELD_TEXTURE.Object;
	}
#pragma endregion

	WeaponMode = EWeaponMode::Weapon;
	NormalWeaponType = ENormalWeaponType::Shield;
	EquipSocketName = "Shield_Equip";
	UnEquipSocketName = "Shield_UnEquip";
	bHandIK = false;

	GuardEfficiency = .8f;

	OnDodgeLineTraceDistance = 350.f;
	SetItemName("Shield");
}

void ABaseShield::Dash(bool bPressed)
{
	SMOOTH_CHECK(MainArmingWeapon);

	MainArmingWeapon->Dash(bPressed);
}

void ABaseShield::OnDashNotify()
{
	SMOOTH_CHECK(MainArmingWeapon);

	MainArmingWeapon->OnDashNotify();
}

void ABaseShield::Attack(bool bPressed)
{
	SMOOTH_CHECK(MainArmingWeapon);

	MainArmingWeapon->Attack(bPressed);
}

void ABaseShield::AttackAssist(bool bPressed)
{
	SMOOTH_CHECK(MainArmingWeapon);

	MainArmingWeapon->AttackAssist(bPressed);
}

bool ABaseShield::CanArmingAsSubWeapon(ABaseWeapon* const InMainArmingWeapon)
{
	if (Cast<ABaseSword>(InMainArmingWeapon))
	{
		return true;
	}

	return false;
}

void ABaseShield::RegisterMainWeapon(ABaseWeapon* const InMainArmingWeapon)
{
	SMOOTH_CHECK(InMainArmingWeapon);

	MainArmingWeapon = InMainArmingWeapon;
}