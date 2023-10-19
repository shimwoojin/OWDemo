// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "OWEnums.h"
#include "CharacterAnimInstance.h"
#include "PlayerAnimInstance.generated.h"

UCLASS()
class OWDEMO_API UPlayerAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void SetZoomAlpha(float Alpha);

protected:
	static float const InterpSpeed;
	static FName const FootL;
	static FName const FootR;
	static float const FootIKInterpSpeed;
	static float const FootIKTraceDistance;

private:
	void CalculateYawPitch();
	void CalculateMoveDirection(float DeltaSeconds);
	void CalculateFootIKAlpha();
	void CalculateLegIK(float DeltaSeconds);
	void TraceForLegIK(const FName& Bone, float& Distance, FRotator& Rotator);
	void CalculateHandIK();
	void CalculateSpineRotation();
	void CalculateHandRLookAtRotator();
	void AdjustHandRRotator();

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
		bool bMoveInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, meta = (AllowPrivateAccess = true))
		float EnableFootIK;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, meta = (AllowPrivateAccess = true))
		FVector TranslationL;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, meta = (AllowPrivateAccess = true))
		FVector TranslationR;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, meta = (AllowPrivateAccess = true))
		FVector TranslationP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, meta = (AllowPrivateAccess = true))
		FRotator RotationL;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootIK, meta = (AllowPrivateAccess = true))
		FRotator RotationR;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandIK, meta = (AllowPrivateAccess = true))
		FTransform HandIKTransform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandIK, meta = (AllowPrivateAccess = true))
		bool bHandIK;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandROnShot, meta = (AllowPrivateAccess = true))
		FRotator HandROnShotRotator;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandROnShot, meta = (AllowPrivateAccess = true))
		bool bAdjustHandRRotator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Angle, meta = (AllowPrivateAccess = true))
		float Yaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Angle, meta = (AllowPrivateAccess = true))
		float Pitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Angle, meta = (AllowPrivateAccess = true))
		float Angle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Angle, meta = (AllowPrivateAccess = true))
		float LastInputAngle;

	float BeforeAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Angle, meta = (AllowPrivateAccess = true))
		FRotator SpineRotator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Zoom, meta = (AllowPrivateAccess = true))
		float ZoomAlpha;
};
