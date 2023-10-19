// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "Blueprint/UserWidget.h"
#include "OWEnums.h"
#include "PlayerUI.generated.h"

UCLASS()
class OWDEMO_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() final;

	void SetGaugePercent(float Percent);
	void SetSkillPercent(float Percent, ESkillType SkillType);
	void SetSkillKeyName(const FString& KeyName, ESkillType SkillType);

	void UpdateSkillCoolTime();

	void SetBlurStrength(float TrueBlurStrength, float FalseBlurStrength, bool bSoldierBlur, bool bWeaponBlur);
	void UpdateItemsToSelectMode(EWeaponMode InWeaponMode);
	void OnChangeMode(EWeaponMode InWeaponMode);
	void UpdateArmingWeaponImage();
	void UpdateSkillImage();
	void UpdateWeaponInfoText();

	void SetHPText(FString InHPText);
	void SetDamageText(FString InDamageText);
	void SetInfoToPlayer(FString InInfoToPlayerText);

	bool IsNormalSkillUIVisible();
	bool IsUltSkillUIVisible();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class UGaugeUI* GaugeUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class USkillUI* NormalSkill;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class USkillUI* UltSkill;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class UImage* ArmingWeaponImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class UImage* ArmingSubWeaponImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class UListView* SelectMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class UBackgroundBlur* SoldierBlur;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class UBackgroundBlur* WeaponBlur;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class UTextBlock* WeaponInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class UTextBlock* HPText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class UTextBlock* DamageText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class UTextBlock* InfoToPlayerText;

private:
	TWeakObjectPtr<class ABasePlayer> Player;
	TWeakObjectPtr<class ABasePlayerController> PlayerController;
};
