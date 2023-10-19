// If you want to use this project, mail to ggoggal@gmail.com


#include "Pistol.h"
#include "Components/CapsuleComponent.h"
#include "particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

APistol::APistol()
{
#pragma region SetupAsset
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PISTOL_MESH
	(TEXT("SkeletalMesh'/Game/Assets/Weapons/MilitaryWeapDark/Weapons/Pistols_B.Pistols_B'"));
	if (PISTOL_MESH.Succeeded())
	{
		GetSkeletalMesh()->SetSkeletalMesh(PISTOL_MESH.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> MUZZLE_FLASH
	(TEXT("ParticleSystem'/Game/Assets/Weapons/MilitaryWeapDark/FX/P_Pistol_MuzzleFlash_01.P_Pistol_MuzzleFlash_01'"));
	if (MUZZLE_FLASH.Succeeded())
	{
		MuzzleFlash->SetTemplate(MUZZLE_FLASH.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SHOOTING_SOUND
	(TEXT("SoundCue'/Game/Assets/Weapons/MilitaryWeapDark/Sound/Pistol/PistolB_Fire_Cue.PistolB_Fire_Cue'"));
	if (SHOOTING_SOUND.Succeeded())
	{
		ShootingSound->SetSound(SHOOTING_SOUND.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BULLET_MESH
	(TEXT("StaticMesh'/Game/Assets/Weapons/MilitaryWeapDark/Weapons/PistolB_Ammo.PistolB_Ammo'"));
	if (BULLET_MESH.Succeeded())
	{
		BulletMesh = BULLET_MESH.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> ZOOM_CURVE
	(TEXT("CurveFloat'/Game/Datas/Curve/BasicLinearFloat.BasicLinearFloat'"));
	if (ZOOM_CURVE.Succeeded())
	{
		ZoomCurve = ZOOM_CURVE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> GUNSHOT_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_PistolFireMontage.DA_PistolFireMontage'"));
	if (GUNSHOT_MONTAGE.Succeeded())
	{
		GunShotMontage = GUNSHOT_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> RELOAD_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_PistolReloadMontage.DA_PistolReloadMontage'"));
	if (RELOAD_MONTAGE.Succeeded())
	{
		ReloadMontage = RELOAD_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> DRAW_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_PistolDrawMontage.DA_PistolDrawMontage'"));
	if (DRAW_MONTAGE.Succeeded())
	{
		DrawMontage = DRAW_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> SHEATH_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_PistolSheathMontage.DA_PistolSheathMontage'"));
	if (SHEATH_MONTAGE.Succeeded())
	{
		SheathMontage = SHEATH_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> PISTOL_TEXTURE
	(TEXT("Texture2D'/Game/Assets/Textures/Weapon/Pistol.Pistol'"));
	if (PISTOL_TEXTURE.Succeeded())
	{
		ItemTexture = PISTOL_TEXTURE.Object;
	}
#pragma endregion

	//SetupComponent
	GetCapsule()->SetCapsuleHalfHeight(22.f);

	//InitProperties
	EquipSocketName = "Pistol_Equip";
	UnEquipSocketName = "Pistol_UnEquip";
	SoldierWeaponType = ESoldierWeaponType::Pistol;
	GunTrigger = EGunTriggerGroup::SingleFire;
	MuzzleSocket = "MuzzleFlash";
	AutoFireRate = .175f;
	SingleFireRate = .2f;
	MaxBullets = 12;
	EquipedBullets = MaxBullets;
	ZoomProperty.bZoomable = false;

	SetItemName("Pistol");
}