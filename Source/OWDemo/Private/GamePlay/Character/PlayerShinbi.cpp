// If you want to use this project, mail to ggoggal@gmail.com


#include "PlayerShinbi.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerShinbi::APlayerShinbi()
{
#pragma region AssetSetup
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SHINBI_BASIC_SKIN(TEXT("SkeletalMesh'/Game/Assets/ParagonShinbi/Characters/Heroes/Shinbi/Meshes/Shinbi.Shinbi'"));
	if (SHINBI_BASIC_SKIN.Succeeded())
	{
		ShinbiBasicSkin = SHINBI_BASIC_SKIN.Object;
		GetMesh()->SetSkeletalMesh(ShinbiBasicSkin);
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SHINBI_SOLDIER_MODE_SKIN(TEXT("SkeletalMesh'/Game/Assets/ParagonShinbi/Characters/Heroes/Shinbi/Skins/Tier_1/Shinbi_Dynasty/Meshes/ShinbiDynasty.ShinbiDynasty'"));
	if (SHINBI_SOLDIER_MODE_SKIN.Succeeded())
	{
		ShinbiSoldierModeSkin = SHINBI_SOLDIER_MODE_SKIN.Object;
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SHINBI_WEAPON_MODE_SKIN(TEXT("SkeletalMesh'/Game/Assets/ParagonShinbi/Characters/Heroes/Shinbi/Skins/Tier_1/Shinbi_Jade/Meshes/ShinbiJade.ShinbiJade'"));
	if (SHINBI_WEAPON_MODE_SKIN.Succeeded())
	{
		ShinbiWeaponModeSkin = SHINBI_WEAPON_MODE_SKIN.Object;
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_BLUEPRINT(TEXT("AnimBlueprint'/Game/Blueprints/Actors/Player/ABP_Shinbi.ABP_Shinbi_C'"));
	if (ANIM_BLUEPRINT.Succeeded())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(ANIM_BLUEPRINT.Class);
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> DODGE_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_BasicDodge.DA_BasicDodge'"));
	if (DODGE_MONTAGE.Succeeded())
	{
		DodgeMontage = DODGE_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> SOLDIER_DODGE_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_SoldierDodge.DA_SoldierDodge'"));
	if (SOLDIER_DODGE_MONTAGE.Succeeded())
	{
		SoldierDodgeMontage = SOLDIER_DODGE_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> CHANGE_MODE_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_ModeChangeMontage.DA_ModeChangeMontage'"));
	if (CHANGE_MODE_MONTAGE.Succeeded())
	{
		ChangeModeMontage = CHANGE_MODE_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> CHANGE_MODE_NONE_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_ModeChangeNoneMontage.DA_ModeChangeNoneMontage'"));
	if (CHANGE_MODE_NONE_MONTAGE.Succeeded())
	{
		ChangeModeNoneMontage = CHANGE_MODE_NONE_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> HITTED_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_HittedMontage.DA_HittedMontage'"));
	if (HITTED_MONTAGE.Succeeded())
	{
		HittedMontage = HITTED_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> PICKUPITEM_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_PickUpItemMontage.DA_PickUpItemMontage'"));
	if (PICKUPITEM_MONTAGE.Succeeded())
	{
		PickUpMontage = PICKUPITEM_MONTAGE.Object;
	}
#pragma endregion
}

void APlayerShinbi::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GetMesh()->HideBoneByName("weapon_r", EPhysBodyOp::PBO_Term);
}

void APlayerShinbi::ChangeSkin(EWeaponMode WeaponMode)
{
	switch (WeaponMode)
	{
	case EWeaponMode::None:
	{
		GetMesh()->SetSkeletalMesh(ShinbiBasicSkin, false);
		break;
	}
	case EWeaponMode::Soldier:
	{
		GetMesh()->SetSkeletalMesh(ShinbiSoldierModeSkin, false);
		break;
	}
	case EWeaponMode::Weapon:
	{
		GetMesh()->SetSkeletalMesh(ShinbiWeaponModeSkin, false);
		break;
	}
	default:
		break;
	}
}