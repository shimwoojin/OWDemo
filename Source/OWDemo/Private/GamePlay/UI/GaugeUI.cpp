// If you want to use this project, mail to ggoggal@gmail.com

#include "GaugeUI.h"
#include "Components/ProgressBar.h"

void UGaugeUI::NativeOnIntialized()
{
	Super::NativeOnInitialized();
}

float UGaugeUI::SetGaugeBarPercent(float Percent)
{
	GaugeBar->SetPercent(Percent);

	return Percent;
}

void UGaugeUI::SetBarFillType(TEnumAsByte<EProgressBarFillType::Type> BarFillType)
{
	GaugeBar->BarFillType = BarFillType;
}

void UGaugeUI::SetBarColor(const FLinearColor& InColor)
{
	GaugeBar->SetFillColorAndOpacity(InColor);
}