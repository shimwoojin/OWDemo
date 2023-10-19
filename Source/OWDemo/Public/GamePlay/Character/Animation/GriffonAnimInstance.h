// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "CharacterAnimInstance.h"
#include "Griffon.h"
#include "GriffonAnimInstance.generated.h"

UCLASS()
class OWDEMO_API UGriffonAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()
	
public:
	void SetGriffonMode(EGriffonMode InGriffonMode) { GriffonMode = InGriffonMode; }
	void SetGriffonState(EGriffonState InGriffonState) { GriffonState = InGriffonState; }
	void SetGriffonSpeed(EGriffonSpeed InGriffonSpeed) { GriffonSpeed = InGriffonSpeed; }

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Griffon, meta = (AllowPrivateAccess = true))
		EGriffonMode GriffonMode;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Griffon, meta = (AllowPrivateAccess = true))
		EGriffonState GriffonState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Griffon, meta = (AllowPrivateAccess = true))
		EGriffonSpeed GriffonSpeed;
};
