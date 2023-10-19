// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "OWEnums.h"
#include "OWStructures.h"
#include "InventoryItem.h"
#include "ActingManagement.h"
#include "AttackManagement.h"
#include "BaseWeapon.generated.h"

UCLASS(Abstract)
class OWDEMO_API ABaseWeapon : 
	public AInventoryItem,
	public IActingManagement,
	public IAttackManagement
{
	GENERATED_BODY()
	
public:	
	ABaseWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:

	//Call When It Owned By Player, Here SetSimulatePhysics(false) -> Effect to Attach, Call Initialize First, Attach to Second
	UFUNCTION(BlueprintCallable)
		virtual void WeaponInitialize(class ABaseCharacter* const Character);

	//LoadSaveData
	virtual bool LoadSaveData(FString InSaveSlotName, int32 InSaveSlotIndex) final;
	virtual void SaveData() final;

	//ActingManagement
	virtual void ActivateSkill(int32 Number, bool bPressed) final;

	//Attach, Detach
	UFUNCTION(BlueprintCallable)
		virtual void AttachTo(class ABaseCharacter* const Character, bool bEquip);

	UFUNCTION(BlueprintCallable)
		virtual void DetachTo();

	//getter, setter
	EWeaponMode GetWeaponMode();
	ESoldierWeaponType GetSoldierWeaponType();
	ENormalWeaponType GetNormalWeaponType();
	void SetSoldierWeaponType(ESoldierWeaponType InType);
	void SetNormalWeaponType(ENormalWeaponType InType);

	//Get Component
	UFUNCTION(BlueprintGetter)
		USkeletalMeshComponent* GetSkeletalMesh();

	UFUNCTION(BlueprintGetter)
		UStaticMeshComponent* GetStaticMesh();

	UFUNCTION(BlueprintGetter)
		UCapsuleComponent* GetCapsule();

	//Damage
	float GetDamage();
	void SetDamage(float InDamage);
	void AddDamage(float AddedDamage);

	virtual float GuardDamageAsPercent() { return 1.f; }
	float GetGuardEfficiency() { return GuardEfficiency; }

	//HandIK
	virtual bool NeedHandIK() { return bHandIK; }
	virtual FName HandIKBoneName() { return HandIK; }

	//Montages
	FMontagePlayData GetDrawMontageData();
	FMontagePlayData GetSheathMontageData();
	class UMontagePlayDataAsset* GetAttackMontage() { return AttackMontage; }
	class UMontagePlayDataAsset* GetAttackAssistMontage() { return AttackAssistMontage; }
	class UMontagePlayDataAsset* GetDodgeMontage() { return DodgeMontage; }

	//Almost Used On Change Weapon Mode
	void Show();
	void Hide();

	//For Sub Weapon
	virtual bool CanArmingAsSubWeapon(ABaseWeapon* const InMainArmingWeapon) { return false; }
	virtual bool CanStandAlone() { return true; }
	virtual void RegisterMainWeapon(ABaseWeapon* const InMainArmingWeapon) {}
	bool SubWeaponExist();
	ABaseWeapon* GetSubWeapon();
	
	//Skill
	class ASkill* GetNormalSkill() { return NormalSkill; }
	class ASkill* GetUltSkill() { return UltSkill; }
	class ASkill* GetActiveSkill() { return ActiveSkill; }
	void SetActiveSkill(class ASkill* InSkill) { ActiveSkill = InSkill; }

private:
	void ToggleMesh();

public:
	UPROPERTY(EditAnywhere, Category = Mesh)
		bool bSkeletalMesh;

	uint8 bInitialized : 1;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, meta = (AllowPrivateAccess = true))
		uint8 Upgrade;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Damage, meta = (AllowPrivateAccess = true))
		float GuardEfficiency;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mode, meta = (AllowPrivateAccess = true))
		EWeaponMode WeaponMode = EWeaponMode::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mode, meta = (AllowPrivateAccess = true))
		ESoldierWeaponType SoldierWeaponType = ESoldierWeaponType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mode, meta = (AllowPrivateAccess = true))
		ENormalWeaponType NormalWeaponType = ENormalWeaponType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attach, meta = (AllowPrivateAccess = true))
		FName EquipSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attach, meta = (AllowPrivateAccess = true))
		FName UnEquipSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HandIK, meta = (AllowPrivateAccess = true))
		FName HandIK;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HandIK, meta = (AllowPrivateAccess = true))
		bool bHandIK;

	UPROPERTY(EditDefaultsOnly, Category = "Swap")
		class UMontagePlayDataAsset* DrawMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Swap")
		class UMontagePlayDataAsset* SheathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
		class UMontagePlayDataAsset* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
		class UMontagePlayDataAsset* AttackAssistMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
		class UMontagePlayDataAsset* DodgeMontage;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Damage, meta = (AllowPrivateAccess = true))
		float Damage;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Skill, meta = (AllowPrivateAccess = true))
		TSubclassOf<class ASkill> NormalSkillClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Skill, meta = (AllowPrivateAccess = true))
		TSubclassOf<class ASkill> UltSkillClass;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Skill, meta = (AllowPrivateAccess = true))
		class ASkill* NormalSkill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Skill, meta = (AllowPrivateAccess = true))
		class ASkill* UltSkill;

	class ASkill* ActiveSkill;

private:
	UPROPERTY(VisibleAnywhere, Category = Collision, BlueprintGetter = GetCapsule, meta = (AllowPrivateAccess = true))
		class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere, Category = Mesh, BlueprintGetter = GetSkeletalMesh, meta = (AllowPrivateAccess = true))
		class USkeletalMeshComponent* MainMesh;

	UPROPERTY(VisibleAnywhere, Category = Mesh, BlueprintGetter = GetStaticMesh, meta = (AllowPrivateAccess = true))
		class UStaticMeshComponent* SubMesh;

};
