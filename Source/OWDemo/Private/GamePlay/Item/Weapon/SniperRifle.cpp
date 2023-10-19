// If you want to use this project, mail to ggoggal@gmail.com

#include "SniperRifle.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Components/CapsuleComponent.h"
#include "SniperGunShotCameraShake.h"

ASniperRifle::ASniperRifle()
{
#pragma region SetupAsset
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SNIPER_RIFLE_MESH
	(TEXT("SkeletalMesh'/Game/Assets/Weapons/MilitaryWeapDark/Weapons/Sniper_Rifle_B.Sniper_Rifle_B'"));
	if (SNIPER_RIFLE_MESH.Succeeded())
	{
		GetSkeletalMesh()->SetSkeletalMesh(SNIPER_RIFLE_MESH.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> MUZZLE_FLASH
	(TEXT("ParticleSystem'/Game/Assets/Weapons/MilitaryWeapDark/FX/P_SniperRifle_MuzzleFlash_01.P_SniperRifle_MuzzleFlash_01'"));
	if (MUZZLE_FLASH.Succeeded())
	{
		MuzzleFlash->SetTemplate(MUZZLE_FLASH.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SHOOTING_SOUND
	(TEXT("SoundCue'/Game/Assets/Weapons/MilitaryWeapDark/Sound/SniperRifle/SniperRifleB_Fire_Cue.SniperRifleB_Fire_Cue'"));
	if (SHOOTING_SOUND.Succeeded())
	{
		ShootingSound->SetSound(SHOOTING_SOUND.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BULLET_MESH
	(TEXT("StaticMesh'/Game/Assets/Weapons/MilitaryWeapDark/Weapons/SniperRifleB_Ammo.SniperRifleB_Ammo'"));
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

	static ConstructorHelpers::FObjectFinder<UTexture2D> SNIPER_TEXTURE
	(TEXT("Texture2D'/Game/Assets/Textures/Weapon/Sniper.Sniper'"));
	if (SNIPER_TEXTURE.Succeeded())
	{
		ItemTexture = SNIPER_TEXTURE.Object;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> ZOOMIN_UI
	(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_SniperZoomUI.BP_SniperZoomUI_C'"));
	if (ZOOMIN_UI.Succeeded())
	{
		ZoomInUIClass = ZOOMIN_UI.Class;
	}
#pragma endregion

	//SetupComponent
	GetCapsule()->SetRelativeLocationAndRotation(FVector(0.f, 20.f, 0.f), FRotator(0.f, 0.f, 90.f));

	//SetupProperties
	MuzzleSocket = "MuzzleFlash";
	EquipSocketName = "SniperRifle_Equip";
	UnEquipSocketName = "SniperRifle_UnEquip";
	SoldierWeaponType = ESoldierWeaponType::SniperRifle;
	GunTrigger = EGunTriggerGroup::SingleFire;
	AutoFireRate = 0.f;
	SingleFireRate = 2.f;
	MaxBullets = 5;
	EquipedBullets = MaxBullets;
	ZoomProperty.bZoomable = true;
	ZoomProperty.ZoomRate = 4.f;
	ZoomProperty.ZoomSpeed = .15f;

	GunShotValidDistance = 10000.f;
	BulletSpeed = 15000.f;

	ChangeableGunTriggers.Add(EGunTriggerGroup::SingleFire);

	GunShotCameraShakeClass = USniperGunShotCameraShake::StaticClass();

	SetItemName("SniperRifle");
}