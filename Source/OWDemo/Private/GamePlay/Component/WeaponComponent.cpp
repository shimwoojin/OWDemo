// If you want to use this project, mail to ggoggal@gmail.com

#include "WeaponComponent.h"
#include "BaseWeapon.h"
#include "BaseCharacter.h"
#include "EquipedWeaponSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponDataSaveGame.h"

UWeaponComponent::UWeaponComponent()
{
	bSoldier = false;
	Weapons.AddDefaulted(LimitOfWeaponCount);
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
	LoadData();
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SaveData();
}

void UWeaponComponent::InsertWeaponToSlot(ABaseWeapon* WantForInsert, int32 InSlot)
{
	if (InSlot < 0 || InSlot > LimitOfWeaponCount - 1)
	{
		Logger::Log("Limit Of Weapon Count is Overed");
		return;
	}

	Weapons[InSlot] = WantForInsert;
}

void UWeaponComponent::DeleteWeaponToSlot(int32 InSlot)
{
	if (InSlot < 0 || InSlot > LimitOfWeaponCount - 1)
	{
		Logger::Log("Limit Of Weapon Count is Overed");
		return;
	}

	if (ArmingWeapon == Weapons[InSlot])
	{
		ArmingWeapon = nullptr;

		if (ArmingSubWeapon)
		{
			ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetOwner());
			if (BaseCharacter)
			{
				ArmingSubWeapon->AttachTo(BaseCharacter, false);
			}
			ArmingSubWeapon = nullptr;
		}
	}

	if (ArmingSubWeapon == Weapons[InSlot])
	{
		ArmingSubWeapon = nullptr;
	}

	Weapons[InSlot]->Destroy();
	Weapons[InSlot] = nullptr;

	OnSwapWeaponDelegate.Broadcast();
}

void UWeaponComponent::DeleteWeaponToSlot(class ABaseWeapon* WantToDelete)
{
	int32 Index = Weapons.Find(WantToDelete);

	if (ArmingWeapon == WantToDelete)
	{
		ArmingWeapon = nullptr;

		if (ArmingSubWeapon)
		{
			ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetOwner());
			if (BaseCharacter)
			{
				ArmingSubWeapon->AttachTo(BaseCharacter, false);
			}
			ArmingSubWeapon = nullptr;
		}
	}

	if (ArmingSubWeapon == WantToDelete)
	{
		ArmingSubWeapon = nullptr;
	}

	Weapons[Index]->Destroy();
	Weapons[Index] = nullptr;

	OnSwapWeaponDelegate.Broadcast();
}

bool UWeaponComponent::SwapWeapon(int32 InSlot)
{
	if (InSlot < 0 || InSlot > LimitOfWeaponCount - 1)
	{
		Logger::Log("Limit Of Weapon Count is Overed");
		return false;
	}

	Slot = InSlot;
	bool bEquipedAsSubWeapon = false;
	auto* const Player = Cast<ABaseCharacter>(GetOwner());
	SMOOTH_CHECK(Player, false);

	if (Player->IsSwapable() == false)
	{
		return false;
	}

	if (Weapons[Slot] == nullptr)
	{
		return false;
	}

	if (ArmingWeapon == Weapons[Slot])	//Using Weapon is Selected -> UnEquip
	{
		if (IsArmingSubWeaponNow())		//Case 1 : Using Weapon is Selected, And SubWeapon is Also Equiped -> SubWeapon, Main Weapon will be Equiped
		{
			FMontagePlayData SheathMontageData = ArmingSubWeapon->GetSheathMontageData();
			FOnMontageEnded OnSheathEnd;
			OnSheathEnd.BindLambda([Player, this](UAnimMontage* AnimMontage, bool bInterrupted) {
				if (!bInterrupted)
				{
					FMontagePlayData SheathMontageData = ArmingWeapon->GetSheathMontageData();
					FOnMontageEnded OnSheathEnd;
					OnSheathEnd.BindLambda([Player, this](UAnimMontage* AnimMontage, bool bInterrupted) {
						if (!bInterrupted)
						{
							Player->SetState(ECharacterState::Idle);
						}
						});
					Player->MontagePlay(SheathMontageData);
					Player->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnSheathEnd, SheathMontageData.AnimMontage);

					//SheathWeaponNotifySetup
					if (Player->OnSheath.IsBound())
					{
						Player->OnSheath.Unbind();
					}
					Player->OnSheath.BindLambda([this, Player]() {

						ArmingWeapon->AttachTo(Player, false);
						ArmingWeapon = nullptr;
						OnSwapWeaponDelegate.Broadcast();

						});
				}
				});
			Player->MontagePlay(SheathMontageData);
			Player->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnSheathEnd, SheathMontageData.AnimMontage);

			//SheathWeaponNotifySetup
			if (Player->OnSheath.IsBound())
			{
				Player->OnSheath.Unbind();
			}
			Player->OnSheath.BindLambda([this, Player]() {

				ArmingSubWeapon->AttachTo(Player, false);
				ArmingSubWeapon = nullptr;
				OnSwapWeaponDelegate.Broadcast();

				});
		}
		//Case 2 : Just Sheath Main Weapon
		else
		{
			FMontagePlayData SheathMontageData = ArmingWeapon->GetSheathMontageData();
			FOnMontageEnded OnSheathEnd;
			OnSheathEnd.BindLambda([Player, this](UAnimMontage* AnimMontage, bool bInterrupted) {
				if (!bInterrupted)
				{
					Player->SetState(ECharacterState::Idle);
				}
				});
			Player->MontagePlay(SheathMontageData);
			Player->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnSheathEnd, SheathMontageData.AnimMontage);

			//SheathWeaponNotifySetup
			if (Player->OnSheath.IsBound())
			{
				Player->OnSheath.Unbind();
			}
			Player->OnSheath.BindLambda([this, Player]() {

				ArmingWeapon->AttachTo(Player, false);
				ArmingWeapon = nullptr;
				OnSwapWeaponDelegate.Broadcast();

				});
		}
	}
	else if (ArmingSubWeapon == Weapons[Slot])	//Case 3 : Just Sheath SubWeapon
	{
		FMontagePlayData SheathMontageData = ArmingSubWeapon->GetSheathMontageData();
		FOnMontageEnded OnSheathEnd;
		OnSheathEnd.BindLambda([Player, this](UAnimMontage* AnimMontage, bool bInterrupted) {
			if (!bInterrupted)
			{
				Player->SetState(ECharacterState::Idle);
			}
			});
		Player->MontagePlay(SheathMontageData);
		Player->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnSheathEnd, SheathMontageData.AnimMontage);

		//SheathWeaponNotifySetup
		if (Player->OnSheath.IsBound())
		{
			Player->OnSheath.Unbind();
		}
		Player->OnSheath.BindLambda([this, Player]() {

			ArmingSubWeapon->AttachTo(Player, false);
			ArmingSubWeapon = nullptr;
			OnSwapWeaponDelegate.Broadcast();

			});
	}
	//Case 4 : Here To Under Almost Do Draw(if needed, with sheath)
	else
	{
		if (ArmingWeapon == nullptr)	//If Current Weapon is None
		{
			if (Weapons[Slot]->CanStandAlone())		//Weapon Can StandAlone, ex. Not Shield, Case 5 : Only Draw
			{
				//DrawMontageSetup
				FMontagePlayData DrawMontageData = Weapons[Slot]->GetDrawMontageData();
				FOnMontageEnded OnDrawEnd;

				OnDrawEnd.BindUObject(this, &ThisClass::OnDrawEnd);
				//DrawMontagePlay
				Player->MontagePlay(DrawMontageData);
				//OnDrawMontageEnd
				Player->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnDrawEnd, DrawMontageData.AnimMontage);
			}
			//ex. Shield Can't Equip As Solo, Case 6 : SubWeapon Can't Draw With Any Other Main Weapon
			else
			{
				return false;
			}
		}
		//If Current Weapon is Not None
		else
		{
			//Case 7 : SubWeapon Draw, if can
			if (Weapons[Slot]->CanArmingAsSubWeapon(ArmingWeapon))	//Current Weapon is Existing, And Current Weapon Matched With SubWeapon
			{
				bEquipedAsSubWeapon = true;
				Weapons[Slot]->RegisterMainWeapon(ArmingWeapon);

				//DrawMontageSetup
				FMontagePlayData DrawMontageData = Weapons[Slot]->GetDrawMontageData();
				FOnMontageEnded OnDrawEnd;

				OnDrawEnd.BindUObject(this, &ThisClass::OnDrawEnd);
				//DrawMontagePlay
				Player->MontagePlay(DrawMontageData);
				//OnDrawMontageEnd
				Player->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnDrawEnd, DrawMontageData.AnimMontage);
			}
			else if (!Weapons[Slot]->CanStandAlone())	//Case 8 : Current Weapon is Existing, Sub Weapon Not Matched With Main Weapon.
			{
				return false;
			}
			//Case 9 : Not Sub Weapon, just Swap As Main Weapon To New Main Weapon
			else
			{
				//SheathMontageSetup
				FMontagePlayData SheathMontageData = ArmingWeapon->GetSheathMontageData();
				FOnMontageEnded OnSheathEnd;

				OnSheathEnd.BindUObject(this, &ThisClass::OnSheathEnd);
				//SheathMontagePlay
				Player->MontagePlay(SheathMontageData);
				//OnSheathMontageEnd
				Player->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnSheathEnd, SheathMontageData.AnimMontage);

				//SheathWeaponNotifySetup
				if (Player->OnSheath.IsBound())
				{
					Player->OnSheath.Unbind();
				}
				Player->OnSheath.BindLambda([this, Player]() {

					ArmingWeapon->AttachTo(Player, false);
					ArmingWeapon = nullptr;
					OnSwapWeaponDelegate.Broadcast();

					});
			}
			
		}

		//DrawWeaponNotifySetup
		if (Player->OnDraw.IsBound())
		{
			Player->OnDraw.Unbind();
		}
		Player->OnDraw.BindLambda([this, Player, bEquipedAsSubWeapon]() {

			Weapons[Slot]->AttachTo(Player, true);
			
			if (bEquipedAsSubWeapon)
			{
				ArmingSubWeapon = Weapons[Slot];
			}
			else
			{
				ArmingWeapon = Weapons[Slot];
			}

			OnSwapWeaponDelegate.Broadcast();

			});

	}

	Player->SetState(ECharacterState::Swap);

	return true;
}

bool UWeaponComponent::IsArmingWeaponNow()
{
	return ArmingWeapon != nullptr;
}

bool UWeaponComponent::IsArmingSubWeaponNow()
{
	return ArmingSubWeapon != nullptr;
}

void UWeaponComponent::GetArmingWeaponType(ESoldierWeaponType& OutSoldierWeaponType, ENormalWeaponType& OutNormalWeaponType)
{
	EWeaponMode WeaponMode;
	ABaseWeapon* WeaponToGet;

	if (IsArmingSubWeaponNow())
	{
		WeaponMode = ArmingSubWeapon->GetWeaponMode();
		WeaponToGet = ArmingSubWeapon;
	}
	else if(IsArmingWeaponNow())
	{
		WeaponMode = ArmingWeapon->GetWeaponMode();
		WeaponToGet = ArmingWeapon;
	}
	else
	{
		return;
	}

	switch (WeaponMode)
	{
	case EWeaponMode::Soldier:
	{
		OutSoldierWeaponType = WeaponToGet->GetSoldierWeaponType();
		break;
	}
	case EWeaponMode::Weapon:
	{
		OutNormalWeaponType = WeaponToGet->GetNormalWeaponType();
		break;
	}
	}
}

ABaseWeapon* UWeaponComponent::GetArmingWeapon()
{
	return ArmingWeapon;
}

ABaseWeapon* UWeaponComponent::GetArmingSubWeapon()
{
	return ArmingSubWeapon;
}

void UWeaponComponent::Dash(bool bPressed)
{
	if (IsArmingSubWeaponNow())
	{
		if (auto* const Weapon = Cast<IActingManagement>(ArmingSubWeapon))
		{
			Weapon->Dash(bPressed);
		}
	}
	else if (IsArmingWeaponNow())
	{
		if (auto* const Weapon = Cast<IActingManagement>(ArmingWeapon))
		{
			Weapon->Dash(bPressed);
		}
	}
}

void UWeaponComponent::OnDashNotify()
{
	if (IsArmingSubWeaponNow())
	{
		if (auto* const Weapon = Cast<IActingManagement>(ArmingSubWeapon))
		{
			Weapon->OnDashNotify();
		}
	}
	else if (IsArmingWeaponNow())
	{
		if (auto* const Weapon = Cast<IActingManagement>(ArmingWeapon))
		{
			Weapon->OnDashNotify();
		}
	}
}

void UWeaponComponent::Attack(bool bPressed)
{
	if (IsArmingSubWeaponNow())
	{
		if (auto* const Weapon = Cast<IActingManagement>(ArmingSubWeapon))
		{
			Weapon->Attack(bPressed);
		}
	}
	else if (IsArmingWeaponNow())
	{
		if (auto* const Weapon = Cast<IActingManagement>(ArmingWeapon))
		{
			Weapon->Attack(bPressed);
		}
	}
}

void UWeaponComponent::AttackAssist(bool bPressed)
{
	if (IsArmingSubWeaponNow())
	{
		if (auto* const Weapon = Cast<IActingManagement>(ArmingSubWeapon))
		{
			Weapon->AttackAssist(bPressed);
		}
	}
	else if (IsArmingWeaponNow())
	{
		if (auto* const Weapon = Cast<IActingManagement>(ArmingWeapon))
		{
			Weapon->AttackAssist(bPressed);
		}
	}
}

void UWeaponComponent::ActivateSkill(int32 Number, bool bPressed)
{
	if (IsArmingSubWeaponNow())
	{
		if (auto* const Weapon = Cast<IActingManagement>(ArmingSubWeapon))
		{
			Weapon->ActivateSkill(Number, bPressed);
		}
	}
	else if (IsArmingWeaponNow())
	{
		if (auto* const Weapon = Cast<IActingManagement>(ArmingWeapon))
		{
			Weapon->ActivateSkill(Number, bPressed);
		}
	}
}

void UWeaponComponent::Reload()
{
	if (IsArmingSubWeaponNow())
	{
		if (auto* const Gun = Cast<IGunActingManagement>(ArmingSubWeapon))
		{
			Gun->Reload();
		}
	}
	else if (IsArmingWeaponNow())
	{
		if (auto* const Gun = Cast<IGunActingManagement>(ArmingWeapon))
		{
			Gun->Reload();
		}
	}
}

void UWeaponComponent::ChangeGunTrigger()
{
	if (IsArmingSubWeaponNow())
	{
		if (auto* const Gun = Cast<IGunActingManagement>(ArmingSubWeapon))
		{
			Gun->ChangeGunTrigger();
		}
	}
	else if (IsArmingWeaponNow())
	{
		if (auto* const Gun = Cast<IGunActingManagement>(ArmingWeapon))
		{
			Gun->ChangeGunTrigger();
		}
	}
}

void UWeaponComponent::OnDrawEnd(UAnimMontage* const InAnimMontage, bool bInterrupted)
{
	if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		if (!bInterrupted)
		{
			BaseCharacter->SetState(ECharacterState::Idle);
		}
	}
}

void UWeaponComponent::OnSheathEnd(UAnimMontage* const InAnimMontage, bool bInterrupted)
{
	if (auto* const BaseCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		if (!bInterrupted)
		{
			if (ArmingSubWeapon)
			{
				BaseCharacter->MontagePlay(ArmingSubWeapon->GetSheathMontageData());

				FMontagePlayData SheathMontageData = ArmingSubWeapon->GetSheathMontageData();
				FOnMontageEnded OnSheathEnd;

				OnSheathEnd.BindUObject(this, &ThisClass::OnSheathEnd);

				BaseCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnSheathEnd, SheathMontageData.AnimMontage);

				//SheathWeaponNotifySetup
				if (BaseCharacter->OnSheath.IsBound())
				{
					BaseCharacter->OnSheath.Unbind();
				}
				BaseCharacter->OnSheath.BindLambda([this, BaseCharacter]() {

					ArmingSubWeapon->AttachTo(BaseCharacter, false);
					ArmingSubWeapon = nullptr;
					OnSwapWeaponDelegate.Broadcast();

					});
			}
			else
			{
				BaseCharacter->MontagePlay(Weapons[Slot]->GetDrawMontageData());

				FMontagePlayData DrawMontageData = Weapons[Slot]->GetDrawMontageData();
				FOnMontageEnded OnDrawEnd;

				OnDrawEnd.BindUObject(this, &ThisClass::OnDrawEnd);

				BaseCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnDrawEnd, DrawMontageData.AnimMontage);
			}
		}
	}
}

FString UWeaponComponent::GetSaveSlotName(int32 Index)
{
	FString SlotName = "EquipedWeapons";

	if (bSoldier)
	{
		SlotName += "_Soldier";
	}
	else
	{
		SlotName += "_Weapon";
	}

	SlotName += FString::FromInt(Index);

	return  SlotName;
}

void UWeaponComponent::LoadData()
{
	UEquipedWeaponSaveGame* EquipedWeaponData = Cast<UEquipedWeaponSaveGame>(UGameplayStatics::CreateSaveGameObject(UEquipedWeaponSaveGame::StaticClass()));

	if (UGameplayStatics::DoesSaveGameExist(GetSaveSlotName(0), 0))
	{
		EquipedWeaponData = Cast<UEquipedWeaponSaveGame>(UGameplayStatics::LoadGameFromSlot(GetSaveSlotName(0), 0));
		
		for (int i = 0; i < EquipedWeaponData->Weapons.Num(); i++)
		{
			if (EquipedWeaponData->Weapons[i].ItemClass)
			{
				FString WeaponDataSlotName = EquipedWeaponData->Weapons[i].ItemUniqueID;
				ABaseWeapon* NewWeapon = GetWorld()->SpawnActor<ABaseWeapon>(EquipedWeaponData->Weapons[i].ItemClass);
				if (NewWeapon)
				{
					InsertWeaponToSlot(NewWeapon, i);
					NewWeapon->WeaponInitialize(Cast<ABaseCharacter>(GetOwner()));
					NewWeapon->AttachTo(Cast<ABaseCharacter>(GetOwner()), false);
					NewWeapon->Hide();
					NewWeapon->LoadSaveData(WeaponDataSlotName, 0);
				}
			}
		}
	}
}

void UWeaponComponent::SaveData()
{
	UEquipedWeaponSaveGame* EquipedWeaponData = Cast<UEquipedWeaponSaveGame>(UGameplayStatics::CreateSaveGameObject(UEquipedWeaponSaveGame::StaticClass()));

	for (int32 i = 0; i < LimitOfWeaponCount; i++)
	{
		FInventoryItemData InventoryItemData;

		if (Weapons[i])
		{
			InventoryItemData.ItemUniqueID = Weapons[i]->GetInventoryItemUniqueID();
			InventoryItemData.ItemClass = Weapons[i]->GetClass();

			Weapons[i]->SaveData();
		}
		else
		{
			InventoryItemData.ItemUniqueID = "None";
			InventoryItemData.ItemClass = nullptr;
		}
		
		EquipedWeaponData->Weapons.Add(InventoryItemData);
	}

	EquipedWeaponData->SaveGameSlotName = GetSaveSlotName(0);
	EquipedWeaponData->SaveGameSlotIndex = 0;

	UGameplayStatics::SaveGameToSlot(EquipedWeaponData, EquipedWeaponData->SaveGameSlotName, EquipedWeaponData->SaveGameSlotIndex);
}
