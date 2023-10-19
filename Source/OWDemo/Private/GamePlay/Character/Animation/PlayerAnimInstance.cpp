// If you want to use this project, mail to ggoggal@gmail.com

#include "PlayerAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BasePlayer.h"
#include "ModeManageComponent.h"
#include "WeaponComponent.h"
#include "BaseWeapon.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseGun.h"
#include "MontagePlayDataAsset.h"

float const UPlayerAnimInstance::InterpSpeed = 15.0f;
FName const UPlayerAnimInstance::FootL = "foot_l";
FName const UPlayerAnimInstance::FootR = "foot_r";
float const UPlayerAnimInstance::FootIKInterpSpeed = 50.0f;
float const UPlayerAnimInstance::FootIKTraceDistance = 50.0f;

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (auto* Player = Cast<ABasePlayer>(TryGetPawnOwner()))
	{
		auto* ModeManager = Player->GetModeManager();
		
		ModeManager->OnChangeWeaponMode.Add(TDelegate<void(EWeaponMode)>::CreateLambda([Player, this](EWeaponMode Mode)
			{
				WeaponMode = Mode;
			}));

		ModeManager->OnChangeSoldierWeaponType.Add(TDelegate<void(ESoldierWeaponType)>::CreateLambda([Player, this](ESoldierWeaponType Type)
			{
				SoldierWeaponType = Type;
			}));

		ModeManager->OnChangeNormalWeaponType.Add(TDelegate<void(ENormalWeaponType)>::CreateLambda([Player, this](ENormalWeaponType Type)
			{
				NormalWeaponType = Type;
			}));
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (TryGetPawnOwner())
	{
		CalculateYawPitch();
		CalculateMoveDirection(DeltaSeconds);
		CalculateFootIKAlpha();
		CalculateLegIK(DeltaSeconds);
		CalculateSpineRotation();
		CalculateHandRLookAtRotator();
		CalculateHandIK();
	}
}

void UPlayerAnimInstance::SetZoomAlpha(float Alpha)
{
	ZoomAlpha = Alpha;
}

void UPlayerAnimInstance::AdjustHandRRotator()
{
	bool bResult = false;

	if (auto* const BasePlayer = Cast<ABasePlayer>(TryGetPawnOwner()))
	{
		if (auto* const CurrentWeaponComponent = BasePlayer->GetModeManager()->GetCurrentWeaponComponent())
		{
			if (auto* const Gun = Cast<ABaseGun>(CurrentWeaponComponent->GetArmingWeapon()))
			{
				const auto& GunShotMontage = Gun->GetGunShotMontage()->GetMontagePlayData(0);

				if (ZoomAlpha < KINDA_SMALL_NUMBER)
				{
					//@TODO : ActiveMontagesMap.Num > 0 means Not GunShotMontage But Other Montage Is Playing
					//Why GunShotMontage Is Not Contained ActiveMontagesMap ? -> GunShotMontage's Length : .2f, Blend Out : .25f
					//If It Makes Some Trouble, Have To Distinct GunShotMontage vs OtherMontages Different Way
					if (Montage_IsPlaying(GunShotMontage.AnimMontage))
					{
						bResult = true;
					}
					else if(IsAnyMontagePlaying())
					{
						bResult = false;
					}
					else
					{
						bResult = true;
					}
				}
			}
		}
	}

	bAdjustHandRRotator = bResult;
}

void UPlayerAnimInstance::CalculateHandRLookAtRotator()
{
	if (auto* const BasePlayer = Cast<ABasePlayer>(TryGetPawnOwner()))
	{
		AdjustHandRRotator();

		if (bAdjustHandRRotator)
		{
			float LineTraceDistance = 5000.f;
			FVector CameraLocation = BasePlayer->GetCamera()->GetComponentLocation();
			FVector CameraForward = BasePlayer->GetCamera()->GetForwardVector();
			FVector LineTraceEnd = CameraLocation + CameraForward * LineTraceDistance;

			FHitResult HitResult;

			GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, LineTraceEnd, ECollisionChannel::ECC_Visibility);

			FVector EndLocation;
			FVector HandRLocation = GetOwningComponent()->GetSocketLocation("hand_r");

			if (HitResult.bBlockingHit)
			{
				EndLocation = HitResult.Location;
			}
			else
			{
				EndLocation = HandRLocation + CameraForward * LineTraceDistance;
			}

			HandROnShotRotator = UKismetMathLibrary::FindLookAtRotation(EndLocation, HandRLocation);
		}
	}
}

void UPlayerAnimInstance::CalculateYawPitch()
{
	if (auto* MyPawn = TryGetPawnOwner())
	{
		FRotator AimRotation = MyPawn->GetBaseAimRotation();
		FRotator PawnRotation = MyPawn->GetActorRotation();
		FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, PawnRotation);

		Yaw = Delta.Yaw;
		Pitch = Delta.Pitch;
	}
}

void UPlayerAnimInstance::CalculateLegIK(float DeltaSeconds)
{
	if (!::IsValid(TryGetPawnOwner()))
	{
		return;
	}

	float DistanceL;
	FRotator RotatorL;

	TraceForLegIK(FootL, DistanceL, RotatorL);

	float DistanceR;
	FRotator RotatorR;

	TraceForLegIK(FootR, DistanceR, RotatorR);

	float const DistanceP = FMath::Min(DistanceL, DistanceR) + 5.f;

	TranslationP.Z = FMath::FInterpTo(TranslationP.Z, DistanceP, DeltaSeconds, InterpSpeed);
	TranslationL.Z = FMath::FInterpTo(TranslationL.Z, DistanceL - DistanceP, DeltaSeconds, InterpSpeed);
	TranslationR.Z = FMath::FInterpTo(TranslationR.Z, DistanceR - DistanceP, DeltaSeconds, InterpSpeed);

	RotationL = FMath::RInterpTo(RotationL, RotatorL, DeltaSeconds, InterpSpeed);
	RotationR = FMath::RInterpTo(RotationR, RotatorR, DeltaSeconds, InterpSpeed);
}

void UPlayerAnimInstance::CalculateMoveDirection(float DeltaSeconds)
{
	BeforeAngle = Angle;

	if (auto* MyCharacter = Cast<ACharacter>(TryGetPawnOwner()))
	{
		FVector Input = MyCharacter->GetCharacterMovement()->GetLastInputVector().GetSafeNormal();

		if (Input.IsZero())
		{
			if (bMoveInput)
			{
				LastInputAngle = BeforeAngle;
			}

			bMoveInput = false;
			return;
		}
		else
		{
			bMoveInput = true;
		}

		FVector Velocity = MyCharacter->GetVelocity();
		FRotator BaseRotation = MyCharacter->GetActorRotation();

		Angle = CalculateDirection(Velocity, BaseRotation);
	}
}

void UPlayerAnimInstance::CalculateFootIKAlpha()
{
	!bCrouched && !bIsInAir ? EnableFootIK = 1.f : EnableFootIK = 0.f;
}

void UPlayerAnimInstance::TraceForLegIK(const FName& Bone, float& Distance, FRotator& Rotator)
{
	FVector const Target = GetOwningComponent()->GetSocketLocation(Bone);
	FVector const Root = GetOwningComponent()->GetSocketLocation("Root");

	FVector const Start = { Target.X, Target.Y, Root.Z + FootIKTraceDistance };
	FVector const End = { Target.X, Target.Y, Root.Z - FootIKTraceDistance };

	FHitResult Result;

	UKismetSystemLibrary::LineTraceSingle
	(
		this,
		Start,
		End,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
		false,
		{ GetOwningActor() },
		DRAW_DEBUG_ONEFRAME,
		Result,
		true
	);

	if (Result.bBlockingHit == true)
	{
		Distance = Result.Location.Z - Root.Z;

		float const LocalRoll = +UKismetMathLibrary::DegAtan2(Result.Normal.Y, Result.Normal.Z);
		float const LocalPitch = -UKismetMathLibrary::DegAtan2(Result.Normal.X, Result.Normal.Z);

		Rotator = { LocalPitch, 0, LocalRoll };
	}
	else
	{
		Distance = 0;
		Rotator = FRotator::ZeroRotator;
	}
}

void UPlayerAnimInstance::CalculateHandIK()
{
	if (auto* const BasePlayer = Cast<ABasePlayer>(TryGetPawnOwner()))
	{
		if (auto* const CurrentWeaponComponent = BasePlayer->GetModeManager()->GetCurrentWeaponComponent())
		{
			if (auto* const ArmingWeapon = CurrentWeaponComponent->GetArmingWeapon())
			{
				bHandIK = ArmingWeapon->NeedHandIK() && !CurrentWeaponComponent->IsArmingSubWeaponNow() && bAdjustHandRRotator;

				if (bHandIK)
				{
					FTransform WeaponHandIKTransform = ArmingWeapon->GetSkeletalMesh()->GetSocketTransform(ArmingWeapon->HandIKBoneName());

					FVector TempLocation;
					FRotator TempRotator;

					GetOwningComponent()->TransformToBoneSpace(
						"hand_r",
						WeaponHandIKTransform.GetLocation(),
						WeaponHandIKTransform.Rotator(),
						TempLocation,
						TempRotator
					);

					HandIKTransform.SetLocation(TempLocation);
					HandIKTransform.SetRotation(TempRotator.Quaternion());

					return;
				}
			}
		}
	}

	bHandIK = false;
}

void UPlayerAnimInstance::CalculateSpineRotation()
{
	float AimYaw = FMath::Clamp(Yaw, -90.f, 90.f);

	SpineRotator = FRotator(0, AimYaw / 4, 0);
}