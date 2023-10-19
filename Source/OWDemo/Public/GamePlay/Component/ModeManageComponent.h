// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "OWEnums.h"
#include "CharacterBaseComponent.h"
#include "ModeManagement.h"
#include "ModeManageComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FChangeWeaponModeDelegate, EWeaponMode);
DECLARE_MULTICAST_DELEGATE_OneParam(FChangeSoldierWeaponTypeDelegate, ESoldierWeaponType);
DECLARE_MULTICAST_DELEGATE_OneParam(FChangeNormalWeaponTypeDelegate, ENormalWeaponType);

UCLASS()
class OWDEMO_API UModeManageComponent : 
	public UCharacterBaseComponent,
	public IModeManagement
{
	GENERATED_BODY()
	
public:
	UModeManageComponent();
	
	virtual void BeginPlay() final;

public:

	UFUNCTION(BlueprintPure)
		class UWeaponComponent* GetCurrentWeaponComponent();

	class UWeaponComponent* GetWeaponModeComponent() { return NormalWeaponMode; }
	class UWeaponComponent* GetSoldierModeComponent() { return SoldierWeaponMode; }

	virtual void ChangeMode() final;
	virtual void ChangeModeNone() final;
	EWeaponMode GetAfterChangeMode(EWeaponMode Before);

	bool SwapWeapon(int32 Slot);
	void OnSwapWeapon();

	UFUNCTION(BlueprintCallable)
		void InsertWeaponToSlot(EWeaponMode InWeaponMode, class ABaseWeapon* const WantForInsert, int32 Slot);

	void DeleteWeaponToSlot(EWeaponMode InWeaponMode, int32 Slot);
	void DeleteWeaponToSlot(EWeaponMode InWeaponMode, class ABaseWeapon* WantToDelete);

private:
	void ChangeArmingWeapon(bool bEquip);
	void ChangeWeaponVisibility(EWeaponMode Before, EWeaponMode After);

public:

	FChangeWeaponModeDelegate OnChangeWeaponMode;
	FChangeSoldierWeaponTypeDelegate OnChangeSoldierWeaponType;
	FChangeNormalWeaponTypeDelegate OnChangeNormalWeaponType;

public:
	EWeaponMode GetWeaponMode() { return WeaponMode; }

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = true))
		EWeaponMode WeaponMode = EWeaponMode::None;

	UPROPERTY()
		class UWeaponComponent* NormalWeaponMode;

	UPROPERTY()
		class UWeaponComponent* SoldierWeaponMode;
};
