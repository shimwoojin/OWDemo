// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "Blueprint/UserWidget.h"
#include "SkillUI.generated.h"

UCLASS()
class OWDEMO_API USkillUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() final;

public:
	void SetBarPercent(float Percent);
	void SetSkillKeyName(const FString& KeyName);
	void SetBackGroundImage(UObject* Image);
	void SetFillImage(UObject* Image);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class UProgressBar* SkillImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class UTextBlock* SkillKey;
};
