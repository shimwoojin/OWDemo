// If you want to use this project, mail to ggoggal@gmail.com

#include "BaseWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "BaseCharacter.h"
#include "MontagePlayDataAsset.h"
#include "BasePlayer.h"
#include "ModeManageComponent.h"
#include "WeaponComponent.h"
#include "Skill.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponDataSaveGame.h"
#include "EquipedWeaponSaveGame.h"

ABaseWeapon::ABaseWeapon()
{
	CreateDefaultSubobjectAuto(Capsule);
	CreateDefaultSubobjectAuto(MainMesh);
	CreateDefaultSubobjectAuto(SubMesh);

	SetRootComponent(MainMesh);
	Capsule->SetupAttachment(GetRootComponent());
	SubMesh->SetupAttachment(GetRootComponent());

	Capsule->SetCollisionProfileName(OWCollision::PresetName_Weapon);
	MainMesh->SetCollisionProfileName(OWCollision::PresetName_InventoryItem);
	MainMesh->SetSimulatePhysics(true);
	SubMesh->SetCollisionProfileName(OWCollision::PresetName_InventoryItem);

	Damage = 30.f;
	Upgrade = 0;
	bSkeletalMesh = true;
	WeaponMode = EWeaponMode::Weapon;
	HandIK = "HandIK";

	ToggleMesh();
	SetItemClassification(EItemClassification::Weapon);
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (NormalSkillClass)
	{
		FActorSpawnParameters SkillSpawnParamter;
		SkillSpawnParamter.Owner = this;
		NormalSkill = GetWorld()->SpawnActor<ASkill>(NormalSkillClass, SkillSpawnParamter);
		if (NormalSkill)
		{
			FAttachmentTransformRules AttachRule(EAttachmentRule::SnapToTarget, false);

			NormalSkill->AttachToActor(this, AttachRule);
		}
	}

	if (UltSkillClass)
	{
		FActorSpawnParameters SkillSpawnParamter;
		SkillSpawnParamter.Owner = this;
		UltSkill = GetWorld()->SpawnActor<ASkill>(UltSkillClass, SkillSpawnParamter);
		if (UltSkill)
		{
			FAttachmentTransformRules AttachRule(EAttachmentRule::SnapToTarget, false);

			UltSkill->AttachToActor(this, AttachRule);
		}
	}
}

void ABaseWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ABaseWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ToggleMesh();
}

void ABaseWeapon::WeaponInitialize(class ABaseCharacter* const Character)
{
	check(Character && "WeaponInitialize fail Because Character is Nullptr!!");

	bInitialized = true;

	SetInstigator(Character);
	SetOwner(Character);
	SetOwned(true);

	if (NormalSkill)
	{
		NormalSkill->SetInstigator(Character);
	}

	if (UltSkill)
	{
		UltSkill->SetInstigator(Character);
	}

	MainMesh->SetSimulatePhysics(false);
	MainMesh->SetCollisionProfileName(OWCollision::PresetName_NoCollision);
	SubMesh->SetCollisionProfileName(OWCollision::PresetName_NoCollision);
}

bool ABaseWeapon::LoadSaveData(FString InSaveSlotName, int32 InSaveSlotIndex)
{
	UWeaponDataSaveGame* LoadGameInstance = Cast<UWeaponDataSaveGame>(UGameplayStatics::CreateSaveGameObject(UWeaponDataSaveGame::StaticClass()));

	if (LoadGameInstance)
	{
		if(UGameplayStatics::DoesSaveGameExist(InSaveSlotName, InSaveSlotIndex))
		{
			LoadGameInstance = Cast<UWeaponDataSaveGame>(UGameplayStatics::LoadGameFromSlot(InSaveSlotName, InSaveSlotIndex));

			SetInventoryItemUniqueID(InSaveSlotName);
			Upgrade = LoadGameInstance->Upgrade;

			return true;
		}
	}

	return false;
}

void ABaseWeapon::SaveData()
{
	UWeaponDataSaveGame* EquipedWeaponData = Cast<UWeaponDataSaveGame>(UGameplayStatics::CreateSaveGameObject(UWeaponDataSaveGame::StaticClass()));

	if (EquipedWeaponData)
	{
		EquipedWeaponData->SaveSlotName = GetInventoryItemUniqueID();
		EquipedWeaponData->SaveSlotIndex = 0;
		EquipedWeaponData->Upgrade = Upgrade;
		UGameplayStatics::SaveGameToSlot(EquipedWeaponData, EquipedWeaponData->SaveSlotName, EquipedWeaponData->SaveSlotIndex);
	}
	else
	{
		Logger::Log(FString::Printf(TEXT("Not Saved WeaponData : %s"), *GetInventoryItemUniqueID()));
	}
}

void ABaseWeapon::ActivateSkill(int32 Number, bool bPressed)
{

	if (bPressed)
	{
		switch (Number)
		{
		case 0:
		{
			if (NormalSkill)
			{
				NormalSkill->Activate();
			}
			break;
		}
		case 1:
		{
			if (UltSkill)
			{
				UltSkill->Activate();
			}
			break;
		}
		default:
		{
			checkNoEntry();
			break;
		}
		}

	}
	else
	{
		switch (Number)
		{
		case 0:
		{
			if (NormalSkill)
			{
				NormalSkill->OnKeyReleased();
			}
			break;
		}
		case 1:
		{
			if (UltSkill)
			{
				UltSkill->OnKeyReleased();
			}
			break;
		}
		default:
		{
			checkNoEntry();
			break;
		}
		}
	}
}

void ABaseWeapon::AttachTo(ABaseCharacter* const Character, bool bEquip)
{
	check(Character && "Attached Character is Null");

	auto* AttachMesh = Character->GetMesh();

	FAttachmentTransformRules AttachRule = FAttachmentTransformRules::SnapToTargetNotIncludingScale;

	if (bEquip)
	{
		AttachToComponent(AttachMesh, AttachRule, EquipSocketName);
	}
	else
	{
		AttachToComponent(AttachMesh, AttachRule, UnEquipSocketName);
	}
}

void ABaseWeapon::DetachTo()
{
	//NullCheck From This Function Internally
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

EWeaponMode ABaseWeapon::GetWeaponMode()
{
	return WeaponMode;
}

ESoldierWeaponType ABaseWeapon::GetSoldierWeaponType()
{
	return SoldierWeaponType;
}

ENormalWeaponType ABaseWeapon::GetNormalWeaponType()
{
	return NormalWeaponType;
}

void ABaseWeapon::SetSoldierWeaponType(ESoldierWeaponType InType)
{
	SoldierWeaponType = InType;
}

void ABaseWeapon::SetNormalWeaponType(ENormalWeaponType InType)
{
	NormalWeaponType = InType;
}

USkeletalMeshComponent* ABaseWeapon::GetSkeletalMesh()
{
	return MainMesh;
}

UStaticMeshComponent* ABaseWeapon::GetStaticMesh()
{
	return SubMesh;
}

UCapsuleComponent* ABaseWeapon::GetCapsule()
{
	return Capsule;
}

float ABaseWeapon::GetDamage()
{
	return Damage;
}

void ABaseWeapon::SetDamage(float InDamage)
{
	if (InDamage < 0.f)
	{
		return;
	}

	Damage = InDamage;
}

void ABaseWeapon::AddDamage(float AddedDamage)
{
	Damage += AddedDamage;
}

FMontagePlayData ABaseWeapon::GetDrawMontageData()
{
	return DrawMontage->GetMontagePlayData(0);
}

FMontagePlayData ABaseWeapon::GetSheathMontageData()
{
	return SheathMontage->GetMontagePlayData(0);
}

void ABaseWeapon::Show()
{
	if (bSkeletalMesh)
	{
		MainMesh->SetHiddenInGame(false);
	}
	else
	{
		SubMesh->SetHiddenInGame(false);
	}

	Capsule->SetCollisionProfileName(OWCollision::PresetName_Weapon, false);
}

void ABaseWeapon::Hide()
{
	if (bSkeletalMesh)
	{
		MainMesh->SetHiddenInGame(true);
	}
	else
	{
		SubMesh->SetHiddenInGame(true);
	}

	Capsule->SetCollisionProfileName(OWCollision::PresetName_NoCollision, false);
}

bool ABaseWeapon::SubWeaponExist()
{
	if (auto* const BasePlayer = Cast<ABasePlayer>(GetOwner()))
	{
		if (auto* const CurrentWeaponComponent = BasePlayer->GetModeManager()->GetCurrentWeaponComponent())
		{
			return CurrentWeaponComponent->IsArmingSubWeaponNow();
		}
	}

	return false;
}

ABaseWeapon* ABaseWeapon::GetSubWeapon()
{
	if (auto* const BasePlayer = Cast<ABasePlayer>(GetOwner()))
	{
		if (auto* const CurrentWeaponComponent = BasePlayer->GetModeManager()->GetCurrentWeaponComponent())
		{
			return CurrentWeaponComponent->GetArmingSubWeapon();
		}
	}

	return nullptr;
}

void ABaseWeapon::ToggleMesh()
{
	bool A;
	bool B;

	if (bSkeletalMesh)
	{
		A = false;
		B = true;
	}
	else
	{
		A = true;
		B = false;
	}

	MainMesh->SetHiddenInGame(A);
	MainMesh->SetVisibility(B);
	SubMesh->SetHiddenInGame(B);
	SubMesh->SetVisibility(A);
}
