// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "OWStructures.h"
#include "BaseCharacter.h"
#include "ModeManagement.h"
#include "ViewManagement.h"
#include "GunActingManagement.h"
#include "InventoryManagement.h"
#include "BasePlayer.generated.h"

struct FChangeModeCameraSetting
{
	FChangeModeCameraSetting(float InTargetArmlength, FVector InSocketOffset, FVector InTargetOffset)
		: TargetArmLength(InTargetArmlength), SocketOffset(InSocketOffset), TargetOffset(InTargetOffset)
	{}

	float TargetArmLength;
	FVector SocketOffset;
	FVector TargetOffset;
};

UCLASS()
class OWDEMO_API ABasePlayer : 
	public ABaseCharacter, 
	public IModeManagement,
	public IViewManagement,
	public IGunActingManagement,
	public IInventoryManagement
{
	GENERATED_BODY()
	
public:
	ABasePlayer();

protected:
	//Default
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) final;

private:
	UFUNCTION()
		void ClearDamageText();

public:
	virtual float AddCurrentHP(float AddHP) final;
	virtual float SetMaxHP(float MaxHP, bool bEffectToCurrentHP) final;

public:

	//View
	virtual void ToggleCameraAngle() final;
	virtual void CameraShake(TSubclassOf<class UMatineeCameraShake> CameraShakeClass) final;

	//Mode
	virtual void ChangeMode() final;
	virtual void ChangeModeNone() final;

	//CharacterDefault
	virtual void Swap(int32 Number) final;

	//ActingManagement
	virtual void Dash(bool bPressed) final;
	virtual void OnDashNotify() final;
	virtual void Attack(bool bPressed) final;
	virtual void AttackAssist(bool bPressed) final;
	virtual void ActivateSkill(int32 Number, bool bPressed) final;

protected:
	virtual void Dodge() final;

public:

	//ActingManagement
	virtual void Reload() final;
	virtual void ChangeGunTrigger() final;

	//AttackManagement
	virtual void StartAttack() override;
	virtual void AttackTick(float DeltaTime, float TotalDuration) override;
	virtual void EndAttack() override;

	//ModeManagement
	virtual void OnNotifyModeChange();
	virtual void OnNotifyModeChangeNone();
	
	//SkillActivateManagement
	virtual void OnSkillStartNotify() final;
	virtual void OnSkillEndNotify() final;

	//InventoryManagement
	virtual bool AddItemToInventory(class AInventoryItem* const WantToAdd) final;
	virtual bool SubItemToInventory(class AInventoryItem* const WantToAdd, int32 Count) final;
	virtual bool SubItemToInventory(EItemClassification InItemClassification, int32 Index, int32 Count) final;

public:
	void ShowGauge();
	void HideGauge();
	void SetGaugePercent(float Percent);

public:
	void Zoom(float Alpha, FZoomProperty ZoomProperty);
	class UCameraComponent* GetCamera();

private:
	void ChangeWeaponMode(EWeaponMode WeaponMode);

	UFUNCTION()
		void OnChangeModeTimeline(float Alpha);

	UFUNCTION()
		void OnChangeModeNoneTimeline(float Alpha);

public:
	class UModeManageComponent* GetModeManager();
	class UInventoryComponent* GetInventoryManager() { return InventoryManager; }

private:

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gauge, meta = (AllowPrivateAccess = true))
		class UWidgetComponent* Gauge;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ModeManager, meta = (AllowPrivateAccess = true))
		class UModeManageComponent* ModeManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = true))
		class UInventoryComponent* InventoryManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timeline, meta = (AllowPrivateAccess = true))
		class UTimelineComponent* ChangeModeTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timeline, meta = (AllowPrivateAccess = true))
		class UTimelineComponent* ChangeModeNoneTimeline;

	UPROPERTY(EditDefaultsOnly)
		class UCurveFloat* ChangeModeCurve;

	TMap<EWeaponMode, FChangeModeCameraSetting> CameraSettingMap;

	bool bChangeModeStart;

private:

	FTimerHandle DashTimerHandle;
	float DashStartDelay;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Dodge)
		class UMontagePlayDataAsset* SoldierDodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category = ModeChange)
		class UMontagePlayDataAsset* ChangeModeMontage;

	UPROPERTY(EditDefaultsOnly, Category = ModeChange)
		class UMontagePlayDataAsset* ChangeModeNoneMontage;
};
