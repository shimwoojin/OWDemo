// If you want to use this project, mail to ggoggal@gmail.com

#include "PlayerUI.h"
#include "GaugeUI.h"
#include "SkillUI.h"
#include "Components/BackgroundBlur.h"
#include "Components/ListView.h"
#include "ItemUIData.h"
#include "BasePlayer.h"
#include "ModeManageComponent.h"
#include "WeaponComponent.h"
#include "BaseWeapon.h"
#include "Components/Image.h"
#include "BasePlayerController.h"
#include "ItemEquipmentUI.h"
#include "InventoryUI.h"
#include "Skill.h"
#include "Components/TextBlock.h"

void UPlayerUI::NativeConstruct()
{
	Super::NativeConstruct();

	Player = Cast<ABasePlayer>(GetOwningPlayerPawn());
	PlayerController = Cast<ABasePlayerController>(GetOwningPlayer());

	if (Player == nullptr || PlayerController == nullptr)
	{
		LOG(Warning, TEXT("PlayerUI, Player = nullptr or PlayerController = nullptr"));
	}

	if (Player.Get())
	{
		UModeManageComponent* ModeManager = Player->GetModeManager();

		if (ModeManager)
		{
			ModeManager->OnChangeWeaponMode.AddUObject(this, &ThisClass::UpdateItemsToSelectMode);
			ModeManager->OnChangeWeaponMode.AddUObject(this, &ThisClass::OnChangeMode);
			ModeManager->GetWeaponModeComponent()->OnSwapWeaponDelegate.AddUObject(this, &ThisClass::UpdateArmingWeaponImage);
			ModeManager->GetWeaponModeComponent()->OnSwapWeaponDelegate.AddUObject(this, &ThisClass::UpdateSkillImage);
			ModeManager->GetWeaponModeComponent()->OnSwapWeaponDelegate.AddUObject(this, &ThisClass::UpdateWeaponInfoText);
			ModeManager->GetSoldierModeComponent()->OnSwapWeaponDelegate.AddUObject(this, &ThisClass::UpdateArmingWeaponImage);
			ModeManager->GetSoldierModeComponent()->OnSwapWeaponDelegate.AddUObject(this, &ThisClass::UpdateSkillImage);
			ModeManager->GetSoldierModeComponent()->OnSwapWeaponDelegate.AddUObject(this, &ThisClass::UpdateWeaponInfoText);

			UpdateItemsToSelectMode(ModeManager->GetWeaponMode());
		}
		else
		{
			LOG(Warning, TEXT("No ModeManager At PlayerUI Init"));
		}
	}

	if (PlayerController.Get())
	{
		PlayerController->GetItemEquipmentUI()->OnEquipmentChange.AddUObject(this, &ThisClass::UpdateItemsToSelectMode);
		PlayerController->GetInventoryUI()->OnWeaponInventoryChange.AddUObject(this, &ThisClass::UpdateItemsToSelectMode);
	}

	GaugeUI->SetBarColor(FLinearColor::Red);
	SetSkillKeyName("E", ESkillType::Normal);
	SetSkillKeyName("Q", ESkillType::Ult);
}

void UPlayerUI::SetGaugePercent(float Percent)
{
	GaugeUI->SetGaugeBarPercent(Percent);
}

void UPlayerUI::SetSkillPercent(float Percent, ESkillType SkillType)
{
	USkillUI* SkillUI = nullptr;

	switch (SkillType)
	{
	case ESkillType::Normal:
	{
		SkillUI = NormalSkill;
		break;
	}
	case ESkillType::Ult:
	{
		SkillUI = UltSkill;
		break;
	}
	}

	if (SkillUI)
	{
		SkillUI->SetBarPercent(Percent);
	}
	else
	{
		LOG(Warning, TEXT("Local SkillUI is Nullptr"));
	}
}

void UPlayerUI::SetSkillKeyName(const FString& KeyName, ESkillType SkillType)
{
	USkillUI* SkillUI = nullptr;

	switch (SkillType)
	{
	case ESkillType::Normal:
	{
		SkillUI = NormalSkill;
		break;
	}
	case ESkillType::Ult:
	{
		SkillUI = UltSkill;
		break;
	}
	}

	if (SkillUI)
	{
		SkillUI->SetSkillKeyName(KeyName);
	}
	else
	{
		LOG(Warning, TEXT("Local SkillUI is Nullptr"));
	}
}

void UPlayerUI::UpdateSkillCoolTime()
{
	if (Player.Get())
	{
		UModeManageComponent* ModeManager = Player->GetModeManager();
		SMOOTH_CHECK(ModeManager);
		UWeaponComponent* CurrentWeaponComponent = ModeManager->GetCurrentWeaponComponent();
		
		if (!CurrentWeaponComponent)
		{
			return;
		}

		if (auto* const ArmingSubWeapon = CurrentWeaponComponent->GetArmingSubWeapon())
		{
			if (auto* const WeaponNormalSkill = ArmingSubWeapon->GetNormalSkill())
			{
				SetSkillPercent(WeaponNormalSkill->GetSkillUsagePercent(), ESkillType::Normal);
			}

			if (auto* const WeaponUltSkill = ArmingSubWeapon->GetUltSkill())
			{
				SetSkillPercent(WeaponUltSkill->GetSkillUsagePercent(), ESkillType::Ult);
			}
		}
		else if (auto* const ArmingWeapon = CurrentWeaponComponent->GetArmingWeapon())
		{
			if (auto* const WeaponNormalSkill = ArmingWeapon->GetNormalSkill())
			{
				SetSkillPercent(WeaponNormalSkill->GetSkillUsagePercent(), ESkillType::Normal);
			}

			if (auto* const WeaponUltSkill = ArmingWeapon->GetUltSkill())
			{
				SetSkillPercent(WeaponUltSkill->GetSkillUsagePercent(), ESkillType::Ult);
			}
		}
	}
}

void UPlayerUI::SetBlurStrength(float TrueBlurStrength, float FalseBlurStrength, bool bSoldierBlur, bool bWeaponBlur)
{
	if (bSoldierBlur)
	{
		SoldierBlur->SetBlurStrength(TrueBlurStrength);
	}
	else
	{
		SoldierBlur->SetBlurStrength(FalseBlurStrength);
	}

	if (bWeaponBlur)
	{
		WeaponBlur->SetBlurStrength(TrueBlurStrength);
	}
	else
	{
		WeaponBlur->SetBlurStrength(FalseBlurStrength);
	}
}

void UPlayerUI::OnChangeMode(EWeaponMode InWeaponMode)
{
	if (InWeaponMode == EWeaponMode::None)
	{
		SelectMode->SetVisibility(ESlateVisibility::Collapsed);
		SetBlurStrength(0.f, 3.f, false, false);
	}
	else
	{
		SelectMode->SetVisibility(ESlateVisibility::Visible);

		if (InWeaponMode == EWeaponMode::Soldier)
		{
			SetBlurStrength(0.f, 3.f, true, false);
		}
		else
		{
			SetBlurStrength(0.f, 3.f, false, true);
		}
	}
}

void UPlayerUI::UpdateItemsToSelectMode(EWeaponMode InWeaponMode)
{
	if (Player.Get())
	{
		if (auto* const CurrentWeaponComponent = Player->GetModeManager()->GetCurrentWeaponComponent())
		{
			SMOOTH_CHECK(CurrentWeaponComponent);

			SelectMode->ClearListItems();

			for (ABaseWeapon* Weapon : CurrentWeaponComponent->GetWeapons())
			{
				UItemUIData* ItemUIData = NewObject<UItemUIData>(this);

				if (Weapon != nullptr)
				{
					ItemUIData->ItemTexture = Weapon->ItemTexture;
					ItemUIData->ItemName = Weapon->GetItemName();
				}

				SelectMode->AddItem(ItemUIData);
			}
		}
	}

	UpdateArmingWeaponImage();
	UpdateSkillImage();
	UpdateWeaponInfoText();
}

void UPlayerUI::UpdateArmingWeaponImage()
{
	if (Player.Get())
	{
		if (auto* const ModeManager = Player->GetModeManager())
		{
			if (auto* const CurrentWeaponComponent = ModeManager->GetCurrentWeaponComponent())
			{
				if (auto* const ArmingWeapon = CurrentWeaponComponent->GetArmingWeapon())
				{
					ArmingWeaponImage->SetBrushFromTexture(ArmingWeapon->ItemTexture);

					if (auto* const ArmingSubWeapon = CurrentWeaponComponent->GetArmingSubWeapon())
					{
						ArmingSubWeaponImage->SetBrushFromTexture(ArmingSubWeapon->ItemTexture);
					}
					else
					{
						ArmingSubWeaponImage->SetBrushFromTexture(UItemUIData::Empty_Image);
					}
				}
				else
				{
					ArmingWeaponImage->SetBrushFromTexture(UItemUIData::Empty_Image);
					ArmingSubWeaponImage->SetBrushFromTexture(UItemUIData::Empty_Image);
				}
			}
			else
			{
				ArmingWeaponImage->SetBrushFromTexture(UItemUIData::Empty_Image);
				ArmingSubWeaponImage->SetBrushFromTexture(UItemUIData::Empty_Image);
			}
		}
	}
}

void UPlayerUI::UpdateSkillImage()
{
	if (Player.Get())
	{
		if (auto* const ModeManager = Player->GetModeManager())
		{
			if (auto* const CurrentWeaponComponent = ModeManager->GetCurrentWeaponComponent())
			{
				if (auto* const ArmingSubWeapon = CurrentWeaponComponent->GetArmingSubWeapon())
				{
					if (auto* const WeaponNormalSkill = ArmingSubWeapon->GetNormalSkill())
					{
						NormalSkill->SetVisibility(ESlateVisibility::HitTestInvisible);
						NormalSkill->SetBackGroundImage(WeaponNormalSkill->SkillTexture);
						NormalSkill->SetFillImage(WeaponNormalSkill->SkillTexture);
					}
					else
					{
						NormalSkill->SetVisibility(ESlateVisibility::Collapsed);
					}

					if (auto* const WeaponUltSkill = ArmingSubWeapon->GetUltSkill())
					{
						UltSkill->SetVisibility(ESlateVisibility::HitTestInvisible);
						UltSkill->SetBackGroundImage(WeaponUltSkill->SkillTexture);
						UltSkill->SetFillImage(WeaponUltSkill->SkillTexture);
					}
					else
					{
						UltSkill->SetVisibility(ESlateVisibility::Collapsed);
					}
				}
				else if (auto* const ArmingWeapon = CurrentWeaponComponent->GetArmingWeapon())
				{
					if (auto* const WeaponNormalSkill = ArmingWeapon->GetNormalSkill())
					{
						NormalSkill->SetVisibility(ESlateVisibility::HitTestInvisible);
						NormalSkill->SetBackGroundImage(WeaponNormalSkill->SkillTexture);
						NormalSkill->SetFillImage(WeaponNormalSkill->SkillTexture);
					}
					else
					{
						NormalSkill->SetVisibility(ESlateVisibility::Collapsed);
					}

					if (auto* const WeaponUltSkill = ArmingWeapon->GetUltSkill())
					{
						UltSkill->SetVisibility(ESlateVisibility::HitTestInvisible);
						UltSkill->SetBackGroundImage(WeaponUltSkill->SkillTexture);
						UltSkill->SetFillImage(WeaponUltSkill->SkillTexture);
					}
					else
					{
						UltSkill->SetVisibility(ESlateVisibility::Collapsed);
					}
				}
				else
				{
					NormalSkill->SetVisibility(ESlateVisibility::Collapsed);
					UltSkill->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
			else
			{
				NormalSkill->SetVisibility(ESlateVisibility::Collapsed);
				UltSkill->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UPlayerUI::UpdateWeaponInfoText()
{
	FText WeaponInfoText;

	if (Player.Get())
	{
		if (auto* const ModeManager = Player->GetModeManager())
		{
			if (auto* const CurrentWeaponComponent = ModeManager->GetCurrentWeaponComponent())
			{
				if (auto* const ArmingSubWeapon = CurrentWeaponComponent->GetArmingSubWeapon())
				{
					WeaponInfoText = FText::FromString(ArmingSubWeapon->GetItemInfo());
				}
				else if (auto* const ArmingWeapon = CurrentWeaponComponent->GetArmingWeapon())
				{
					WeaponInfoText = FText::FromString(ArmingWeapon->GetItemInfo());
				}
				else
				{
					//DoNothing	
				}
			}
			else
			{
				//DoNothing
			}

			WeaponInfo->SetText(WeaponInfoText);
		}
	}
}

void UPlayerUI::SetHPText(FString InHPText)
{
	HPText->SetText(FText::FromString(InHPText));
}

void UPlayerUI::SetDamageText(FString InDamageText)
{
	DamageText->SetText(FText::FromString(InDamageText));
}

void UPlayerUI::SetInfoToPlayer(FString InInfoToPlayerText)
{
	InfoToPlayerText->SetText(FText::FromString(InInfoToPlayerText));
}

bool UPlayerUI::IsNormalSkillUIVisible()
{
	return (NormalSkill->GetVisibility() == ESlateVisibility::Visible) ||
		(NormalSkill->GetVisibility() == ESlateVisibility::HitTestInvisible) ||
		(NormalSkill->GetVisibility() == ESlateVisibility::SelfHitTestInvisible);
}

bool UPlayerUI::IsUltSkillUIVisible()
{
	return (UltSkill->GetVisibility() == ESlateVisibility::Visible) ||
		(UltSkill->GetVisibility() == ESlateVisibility::HitTestInvisible) ||
		(UltSkill->GetVisibility() == ESlateVisibility::SelfHitTestInvisible);
}