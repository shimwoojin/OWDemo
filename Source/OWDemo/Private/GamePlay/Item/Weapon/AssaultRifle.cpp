// If you want to use this project, mail to ggoggal@gmail.com


#include "AssaultRifle.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Components/CapsuleComponent.h"

AAssaultRifle::AAssaultRifle()
{
#pragma region SetupAsset
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RIFLE_AK47
	(TEXT("SkeletalMesh'/Game/Assets/Weapons/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SK_KA47.SK_KA47'"));
	if (RIFLE_AK47.Succeeded())
	{
		GetSkeletalMesh()->SetSkeletalMesh(RIFLE_AK47.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> MUZZLE_FLASH
	(TEXT("ParticleSystem'/Game/Assets/Weapons/MilitaryWeapDark/FX/P_AssaultRifle_MuzzleFlash.P_AssaultRifle_MuzzleFlash'"));
	if (MUZZLE_FLASH.Succeeded())
	{
		MuzzleFlash->SetTemplate(MUZZLE_FLASH.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SHOOTING_SOUND
	(TEXT("SoundCue'/Game/Assets/Weapons/MilitaryWeapDark/Sound/Rifle/RifleB_Fire_Cue.RifleB_Fire_Cue'"));
	if (SHOOTING_SOUND.Succeeded())
	{
		ShootingSound->SetSound(SHOOTING_SOUND.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BULLET_MESH
	(TEXT("StaticMesh'/Game/Assets/Weapons/MilitaryWeapDark/Weapons/AssaultRifleB_Ammo.AssaultRifleB_Ammo'"));
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
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_AssaultRifleFireMontage.DA_AssaultRifleFireMontage'"));
	if (GUNSHOT_MONTAGE.Succeeded())
	{
		GunShotMontage = GUNSHOT_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> RELOAD_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_AssaultRifleReloadMontage.DA_AssaultRifleReloadMontage'"));
	if (RELOAD_MONTAGE.Succeeded())
	{
		ReloadMontage = RELOAD_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> DRAW_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_AssaultiRifleDrawMontage.DA_AssaultiRifleDrawMontage'"));
	if (DRAW_MONTAGE.Succeeded())
	{
		DrawMontage = DRAW_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMontagePlayDataAsset> SHEATH_MONTAGE
	(TEXT("MontagePlayDataAsset'/Game/Datas/DataAsset/MontagePlay/DA_AssaultiRifleSheathMontage.DA_AssaultiRifleSheathMontage'"));
	if (SHEATH_MONTAGE.Succeeded())
	{
		SheathMontage = SHEATH_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> AK47_TEXTURE
	(TEXT("Texture2D'/Game/Assets/Textures/Weapon/AK47.AK47'"));
	if (AK47_TEXTURE.Succeeded())
	{
		ItemTexture = AK47_TEXTURE.Object;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> ZOOMIN_UI
	(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_AssaultRifleZoomUI.BP_AssaultRifleZoomUI_C'"));
	if (ZOOMIN_UI.Succeeded())
	{
		ZoomInUIClass = ZOOMIN_UI.Class;
	}
#pragma endregion

	//SetupComponent
	GetCapsule()->SetRelativeLocationAndRotation(FVector(0.f, 20.f, 0.f), FRotator(0.f, 0.f, 90.f));

	//SetupProperties
	EquipSocketName = "AssaultRifle_Equip";
	UnEquipSocketName = "AssaultRifle_UnEquip";
	SoldierWeaponType = ESoldierWeaponType::AssaultRifle;
	GunTrigger = EGunTriggerGroup::AutoFire;
	AutoFireRate = .125f;
	SingleFireRate = .125f;
	MaxBullets = 30;
	EquipedBullets = 30;
	ZoomProperty.bZoomable = true;
	ZoomProperty.ZoomRate = 1.5f;
	ZoomProperty.ZoomSpeed = .2f;

	ChangeableGunTriggers.Add(EGunTriggerGroup::AutoFire);

	SetItemName("AssaultRifle");
}
