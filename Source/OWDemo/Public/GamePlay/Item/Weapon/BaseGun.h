// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "OWEnums.h"
#include "OWStructures.h"
#include "RangedWeapon.h"
#include "GunActingManagement.h"
#include "BaseGun.generated.h"

UCLASS(Abstract)
class OWDEMO_API ABaseGun :
	public ARangedWeapon,
	public IGunActingManagement
{
	GENERATED_BODY()
	
public:
	ABaseGun();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

public:
	virtual void WeaponInitialize(class ABaseCharacter* const Character) override;
	virtual FString GetItemInfo() final;

public:
	//Acting
	virtual void Attack(bool bPressed) final;
	virtual void AttackAssist(bool bPressed) final;

	//GunActing
	virtual void Reload() final;
	virtual void ChangeGunTrigger() final;

	UFUNCTION()
		void ReloadSuccess(class UAnimMontage* InReloadMontage, bool bInterrupted);

	//OnAttack
	virtual void Fire();
	virtual void StopFire();

	UFUNCTION()
		void OnFireEnd(class UAnimMontage* InAnimMontage, bool bInterrupted);

	//OnAttackAssist
	virtual void ZoomIn();
	virtual void ZoomOut();

	//Left Hand Need to Change Transform(Except Scale) to Gun's "HANDIK" Socket
	virtual bool NeedHandIK() final { return true; }
	class UMontagePlayDataAsset* GetGunShotMontage() { return GunShotMontage; }

private:
	void SpawnBullet();

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = Muzzle)
		FName MuzzleSocket;

	UPROPERTY()
		TArray<EGunTriggerGroup> ChangeableGunTriggers = { EGunTriggerGroup::SingleFire };
	EGunTriggerGroup GunTrigger = ChangeableGunTriggers[0];

	UPROPERTY(EditDefaultsOnly, Category = GunProperty)
		float AutoFireRate;

	UPROPERTY(EditDefaultsOnly, Category = GunProperty)
		float SingleFireRate;

	UPROPERTY(EditDefaultsOnly, Category = GunProperty)
		float GunShotValidDistance;

	UPROPERTY(EditDefaultsOnly, Category = GunProperty)
		float BulletSpeed;

	UPROPERTY(EditDefaultsOnly, Category = GunProperty)
		int32 EquipedBullets;

	UPROPERTY(EditDefaultsOnly, Category = GunProperty)
		int32 MaxBullets;
	
	bool bCanFire;

	FZoomProperty ZoomProperty;
	FTimerHandle FireHandle;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* MuzzleFlash;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = true))
		class UAudioComponent* ShootingSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BulletMesh, meta = (AllowPrivateAccess = true))
		class UStaticMesh* BulletMesh;

	TSharedPtr<struct FTimeline> ZoomTimeline;

	UPROPERTY(EditDefaultsOnly, Category = "Zoom")
		class UCurveFloat* ZoomCurve;

	UPROPERTY(EditDefaultsOnly, Category = Zoom)
		class UUserWidget* ZoomInUI;

	UPROPERTY(EditDefaultsOnly, Category = Zoom)
		TSubclassOf<class UUserWidget> ZoomInUIClass;

	UPROPERTY(EditDefaultsOnly, Category = "GunShot")
		class UMontagePlayDataAsset* GunShotMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Reload")
		class UMontagePlayDataAsset* ReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = CameraShake)
		TSubclassOf<class UMatineeCameraShake> GunShotCameraShakeClass;

};
