// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "OWEnums.h"
#include "CharacterBaseComponent.h"
#include "ActingManagement.h"
#include "GunActingManagement.h"
#include "WeaponComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FSwapWeaponDelegate);

UCLASS()
class OWDEMO_API UWeaponComponent : 
	public UCharacterBaseComponent,
	public IActingManagement,
	public IGunActingManagement
{
	GENERATED_BODY()
	
public:
	UWeaponComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:

	void InsertWeaponToSlot(class ABaseWeapon* WantForInsert, int32 InSlot);
	void DeleteWeaponToSlot(int32 InSlot);
	void DeleteWeaponToSlot(class ABaseWeapon* WantToDelete);
	bool SwapWeapon(int32 Slot);
	bool IsArmingWeaponNow();
	bool IsArmingSubWeaponNow();
	void GetArmingWeaponType(ESoldierWeaponType& OutSoldierWeaponType, ENormalWeaponType& OutNormalWeaponType);
	class ABaseWeapon* GetArmingWeapon();
	class ABaseWeapon* GetArmingSubWeapon();

	//ActingManagement
	virtual void Dash(bool bPressed) final;
	virtual void OnDashNotify() final;
	virtual void Attack(bool bPressed) final;
	virtual void AttackAssist(bool bPressed) final;
	virtual void ActivateSkill(int32 Number, bool bPressed) final;

	//GunActingManagement
	virtual void Reload() final;
	virtual void ChangeGunTrigger() final;

	UFUNCTION()
		void OnDrawEnd(UAnimMontage* const InAnimMontage, bool bInterrupted);
	UFUNCTION()
		void OnSheathEnd(UAnimMontage* const InAnimMontage, bool bInterrupted);

	//Getter
	TArray<class ABaseWeapon*> GetWeapons() { return Weapons; }

private:
	FString GetSaveSlotName(int32 Index);
	void LoadData();
	void SaveData();

protected:
	static int32 const LimitOfWeaponCount = 4;

public:
	bool bSoldier;
	FSwapWeaponDelegate OnSwapWeaponDelegate;

private:

	UPROPERTY()
		class ABaseWeapon* ArmingWeapon;

	UPROPERTY()
		class ABaseWeapon* ArmingSubWeapon;

	UPROPERTY()
		TArray<class ABaseWeapon*> Weapons;

	int32 Slot;
};