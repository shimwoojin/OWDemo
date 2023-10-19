// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "GaugeUI.generated.h"

UCLASS()
class OWDEMO_API UGaugeUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnIntialized() final;

public:
	float SetGaugeBarPercent(float Percent);
	void SetBarFillType(TEnumAsByte<EProgressBarFillType::Type> BarFillType);

	UFUNCTION(BlueprintCallable)
		void SetBarColor(const FLinearColor& InColor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
		class UProgressBar* GaugeBar;

};
