// If you want to use this project, mail to ggoggal@gmail.com

#include "SkillUI.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void USkillUI::NativeConstruct()
{
	Super::NativeConstruct();

}

void USkillUI::SetBarPercent(float Percent)
{
	SkillImage->SetPercent(Percent);
}

void USkillUI::SetSkillKeyName(const FString& KeyName)
{
	SkillKey->SetText(FText::FromString(KeyName));
}

void USkillUI::SetBackGroundImage(UObject* Image)
{
	SkillImage->WidgetStyle.BackgroundImage.SetResourceObject(Image);
}

void USkillUI::SetFillImage(UObject* Image)
{
	SkillImage->WidgetStyle.FillImage.SetResourceObject(Image);
}