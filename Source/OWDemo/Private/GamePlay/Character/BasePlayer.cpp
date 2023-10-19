// If you want to use this project, mail to ggoggal@gmail.com

#include "BasePlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerAnimInstance.h"
#include "ModeManageComponent.h"
#include "StateComponent.h"
#include "WeaponComponent.h"
#include "MontagePlayDataAsset.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"
#include "GaugeUI.h"
#include "Components/ProgressBar.h"
#include "BaseWeapon.h"
#include "InventoryComponent.h"
#include "BasePlayerController.h"
#include "PlayerUI.h"
#include "StatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InventoryUI.h"

ABasePlayer::ABasePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

#pragma region AssetSetup
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CHANGE_MODE_CURVE
	(TEXT("CurveFloat'/Game/Datas/Curve/BasicLinearFloat.BasicLinearFloat'"));
	if (CHANGE_MODE_CURVE.Succeeded())
	{
		ChangeModeCurve = CHANGE_MODE_CURVE.Object;
	}
#pragma endregion

	//Default Player Setup
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->CrouchedHalfHeight = 65.f;

	//New Components Make
	CreateDefaultSubobjectAuto(SpringArm);
	CreateDefaultSubobjectAuto(Camera);
	CreateDefaultSubobjectAuto(ModeManager);
	CreateDefaultSubobjectAuto(InventoryManager);
	CreateDefaultSubobjectAuto(ChangeModeTimeline);
	CreateDefaultSubobjectAuto(ChangeModeNoneTimeline);
	CreateDefaultSubobjectAuto(Gauge);

	SpringArm->SetupAttachment(GetRootComponent());
	Camera->SetupAttachment(SpringArm);

	//SpringArm Setup
	ChangeWeaponMode(EWeaponMode::None);
	SpringArm->bUsePawnControlRotation = true;

	//SpringArm Setup_CameraLag
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 10;
	SpringArm->CameraRotationLagSpeed = 20;
	SpringArm->CameraLagMaxDistance = 50;

	//PropertySetup
	DashStartDelay = 0.15f;

	if (Gauge)
	{
		Gauge->SetupAttachment(GetRootComponent());

		static ConstructorHelpers::FClassFinder<UUserWidget> GAUGE_WIDGET
		(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_GaugeUI.BP_GaugeUI_C'"));

		if (GAUGE_WIDGET.Succeeded())
		{
			Gauge->SetWidgetClass(GAUGE_WIDGET.Class);
			Gauge->SetWidgetSpace(EWidgetSpace::Screen);
			Gauge->SetDrawSize(FVector2D(20.f, 200.f));
			Gauge->SetRelativeLocationAndRotation(FVector(0.f, 30.f, -20.f), FRotator(0.f, 0.f, 90.f));
		}
	}
}

void ABasePlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CameraSettingMap.Add(EWeaponMode::None, FChangeModeCameraSetting(400.f, FVector(0.f, 0.f, 0.f), FVector(0.f, 0.f, 80.f)));
	CameraSettingMap.Add(EWeaponMode::Soldier, FChangeModeCameraSetting(250.f, FVector(0.f, 70.f, 0.f), FVector(0.f, 0.f, 100.f)));
	CameraSettingMap.Add(EWeaponMode::Weapon, FChangeModeCameraSetting(300.f, FVector(0.f, -70.f, 0.f), FVector(0.f, 0.f, 100.f)));

	if (ChangeModeCurve != nullptr && ChangeModeMontage != nullptr && ChangeModeNoneMontage != nullptr)
	{
		float ChangeModeTime = ChangeModeMontage->GetMontagePlayData(0).AnimMontage->GetPlayLength();
		float ChangeModeNoneTime = ChangeModeNoneMontage->GetMontagePlayData(0).AnimMontage->GetPlayLength();

		FOnTimelineFloat ChangeModeTimelineDelegate;
		ChangeModeTimelineDelegate.BindDynamic(this, &ThisClass::OnChangeModeTimeline);

		FOnTimelineFloat ChangeModeNoneTimelineDelegate;
		ChangeModeNoneTimelineDelegate.BindDynamic(this, &ThisClass::OnChangeModeNoneTimeline);

		ChangeModeTimeline->AddInterpFloat(ChangeModeCurve, ChangeModeTimelineDelegate);
		ChangeModeTimeline->SetPlayRate(1 / ChangeModeTime);

		ChangeModeNoneTimeline->AddInterpFloat(ChangeModeCurve, ChangeModeNoneTimelineDelegate);
		ChangeModeNoneTimeline->SetPlayRate(1 / ChangeModeNoneTime);
	}
}

void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();

	if (auto* const GaugeWidget = Cast<UGaugeUI>(Gauge->GetWidget()))
	{
		GaugeWidget->SetBarFillType(EProgressBarFillType::BottomToTop);
		HideGauge();
	}

	SetMaxHP(500.f, true);
}

void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsFalling())
	{
		State->SetGaitState(EGaitState::Jump);
	}
	else
	{
		if (State->IsSameGaitState(EGaitState::Jump))
		{
			State->SetGaitState(EGaitState::Stand);
		}
	}
}

float ABasePlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageFinal = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	
	if (auto* const BasePlayerController = Cast<ABasePlayerController>(GetController()))
	{
		BasePlayerController->GetPlayerUI()->SetDamageText("-" + FString::FromInt(DamageFinal));

		FLatentActionInfo LatentActionInfo;
		LatentActionInfo.CallbackTarget = this;
		LatentActionInfo.ExecutionFunction = "ClearDamageText";
		LatentActionInfo.Linkage = 1;
		LatentActionInfo.UUID = GetActorGuid().D;

		UKismetSystemLibrary::RetriggerableDelay(this, 2.f, LatentActionInfo);
	}

	return DamageFinal;
}

void ABasePlayer::ClearDamageText()
{
	if (auto* const BasePlayerController = Cast<ABasePlayerController>(GetController()))
	{
		BasePlayerController->GetPlayerUI()->SetDamageText("");
	}
}

float ABasePlayer::AddCurrentHP(float AddHP)
{
	float CurrentHP = Super::AddCurrentHP(AddHP);

	if (auto* const BasePlayerController = Cast<ABasePlayerController>(GetController()))
	{
		FStat MaxHP = Stat->GetStat(EStat::MaxHP);

		BasePlayerController->GetPlayerUI()->SetGaugePercent(CurrentHP / MaxHP.Value);
		BasePlayerController->GetPlayerUI()->SetHPText(FString::FromInt(CurrentHP) + " / " + FString::FromInt(MaxHP.Value));
	}

	return CurrentHP;
}

float ABasePlayer::SetMaxHP(float MaxHP, bool bEffectToCurrentHP)
{
	float LocalMaxHP = Super::SetMaxHP(MaxHP, bEffectToCurrentHP);

	if (auto* const BasePlayerController = Cast<ABasePlayerController>(GetController()))
	{
		FStat CurrentHP = Stat->GetStat(EStat::CurrentHP);

		BasePlayerController->GetPlayerUI()->SetGaugePercent(CurrentHP.Value / LocalMaxHP);
		BasePlayerController->GetPlayerUI()->SetHPText(FString::FromInt(CurrentHP.Value) + " / " + FString::FromInt(LocalMaxHP));
	}

	return LocalMaxHP;
}

void ABasePlayer::ToggleCameraAngle()
{
	bool bToggle = GetCharacterMovement()->bOrientRotationToMovement;
	GetCharacterMovement()->bOrientRotationToMovement = !bToggle;
}

void ABasePlayer::CameraShake(TSubclassOf<class UMatineeCameraShake> CameraShakeClass)
{
	Cast<APlayerController>(GetController())->PlayerCameraManager->StartMatineeCameraShake(CameraShakeClass);
}

void ABasePlayer::ChangeMode()
{
	if (IsModeChangeable())
	{
		FMontagePlayData ChangeModeMontageData = ChangeModeMontage->GetMontagePlayData(0);

		if (ChangeModeMontageData.AnimMontage)
		{
			MontagePlay(ChangeModeMontageData);
			SetState(ECharacterState::ModeChange);
			bChangeModeStart = true;
			ChangeModeTimeline->PlayFromStart();

			FOnMontageEnded OnMontageEnded;
			OnMontageEnded.BindLambda([this](UAnimMontage* AnimMontage, bool bInterrupted) {
				
				if (!bInterrupted)
				{
					SetState(ECharacterState::Idle);
				}

				});

			GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded, ChangeModeMontageData.AnimMontage);
		}
	}
}

void ABasePlayer::ChangeModeNone()
{
	if (IsModeChangeable())
	{
		FMontagePlayData ChangeModeNoneMontageData = ChangeModeNoneMontage->GetMontagePlayData(0);

		MontagePlay(ChangeModeNoneMontageData);
		SetState(ECharacterState::ModeChange);
		bChangeModeStart = true;
		ChangeModeNoneTimeline->PlayFromStart();

		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindLambda([this](UAnimMontage* AnimMontage, bool bInterrupted) {

			if (!bInterrupted)
			{
				SetState(ECharacterState::Idle);
			}

			});

		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded, ChangeModeNoneMontageData.AnimMontage);
	}
}

void ABasePlayer::Swap(int32 Number)
{
	SMOOTH_CHECK(ModeManager);

	ModeManager->SwapWeapon(Number);
}

void ABasePlayer::Dash(bool bPressed)
{
	if (bPressed)
	{
		FTimerDelegate DashDelegate;
		DashDelegate.BindUObject(this, &ThisClass::Sprint, bPressed);
		GetWorldTimerManager().SetTimer(DashTimerHandle, DashDelegate, DashStartDelay, false);
	}
	else
	{
		if (GetWorldTimerManager().GetTimerRemaining(DashTimerHandle) != -1.f)
		{
			Dodge();
			GetWorldTimerManager().ClearTimer(DashTimerHandle);
		}
		else
		{
			Sprint(bPressed);
		}
	}
}

void ABasePlayer::OnDashNotify()
{
	SMOOTH_CHECK(ModeManager);

	auto* CurrentWeaponComponent = ModeManager->GetCurrentWeaponComponent();

	if (CurrentWeaponComponent)
	{
		CurrentWeaponComponent->OnDashNotify();
	}
}

void ABasePlayer::Attack(bool bPressed)
{
	SMOOTH_CHECK(ModeManager);

	auto* CurrentWeaponComponent = ModeManager->GetCurrentWeaponComponent();

	if (CurrentWeaponComponent)
	{
		CurrentWeaponComponent->Attack(bPressed);
	}
}

void ABasePlayer::AttackAssist(bool bPressed)
{
	SMOOTH_CHECK(ModeManager);

	auto* CurrentWeaponComponent = ModeManager->GetCurrentWeaponComponent();

	if (CurrentWeaponComponent)
	{
		CurrentWeaponComponent->AttackAssist(bPressed);
	}
}

void ABasePlayer::ActivateSkill(int32 Number, bool bPressed)
{
	SMOOTH_CHECK(ModeManager);

	auto* CurrentWeaponComponent = ModeManager->GetCurrentWeaponComponent();

	if (CurrentWeaponComponent)
	{
		CurrentWeaponComponent->ActivateSkill(Number, bPressed);
	}
}

void ABasePlayer::OnSkillStartNotify()
{
	SMOOTH_CHECK(ModeManager);

	auto* CurrentWeaponComponent = ModeManager->GetCurrentWeaponComponent();

	if (CurrentWeaponComponent)
	{
		if (CurrentWeaponComponent->IsArmingSubWeaponNow())
		{
			if (auto* const Skill = Cast<ISkillActivateManagement>(CurrentWeaponComponent->GetArmingSubWeapon()->GetActiveSkill()))
			{
				Skill->OnSkillStartNotify();
			}
		}
		else if (CurrentWeaponComponent->IsArmingWeaponNow())
		{
			if (auto* const Skill = Cast<ISkillActivateManagement>(CurrentWeaponComponent->GetArmingWeapon()->GetActiveSkill()))
			{
				Skill->OnSkillStartNotify();
			}
		}
	}
}

void ABasePlayer::OnSkillEndNotify()
{
	SMOOTH_CHECK(ModeManager);

	auto* CurrentWeaponComponent = ModeManager->GetCurrentWeaponComponent();

	if (CurrentWeaponComponent)
	{
		if (CurrentWeaponComponent->IsArmingSubWeaponNow())
		{
			if (auto* const Skill = Cast<ISkillActivateManagement>(CurrentWeaponComponent->GetArmingSubWeapon()->GetActiveSkill()))
			{
				Skill->OnSkillEndNotify();
			}
		}
		else if (CurrentWeaponComponent->IsArmingWeaponNow())
		{
			if (auto* const Skill = Cast<ISkillActivateManagement>(CurrentWeaponComponent->GetArmingWeapon()->GetActiveSkill()))
			{
				Skill->OnSkillEndNotify();
			}
		}
	}
}

void ABasePlayer::Dodge()
{
	float Dot = GetActorForwardVector().CosineAngle2D(GetLastMovementInputVector());
	FVector Cross = FVector::CrossProduct(GetActorForwardVector(), GetLastMovementInputVector());

	FMontagePlayData SelectedDodge;

	if (State->IsDashable())
	{
		if (ModeManager->GetWeaponMode() == EWeaponMode::None)
		{

			if (Dot > .1f || GetLastMovementInputVector() == FVector::ZeroVector)	//forward
			{
				SelectedDodge = DodgeMontage->FindMontagePlayData("forward");
			}
			else if (Dot < -.1f)	//backward
			{
				SelectedDodge = DodgeMontage->FindMontagePlayData("backward");
			}
			else	//left or right
			{
				if (Cross.Z > 0.f)
				{
					SelectedDodge = DodgeMontage->FindMontagePlayData("right");
				}
				else
				{
					SelectedDodge = DodgeMontage->FindMontagePlayData("left");
				}
			}
		}
		else if (ModeManager->GetWeaponMode() == EWeaponMode::Soldier)
		{
			FRotator UpdatingRotator;
			
			if (GetLastMovementInputVector() == FVector::ZeroVector)
			{
				UpdatingRotator = GetActorRotation();
			}
			else
			{
				UpdatingRotator = UKismetMathLibrary::MakeRotFromXZ(GetLastMovementInputVector(), GetActorUpVector());
			}

			SetActorRotation(FRotator(0.f, UpdatingRotator.Yaw, -0.f));

			SelectedDodge = SoldierDodgeMontage->GetMontagePlayData(0);
		}
		else if (ModeManager->GetWeaponMode() == EWeaponMode::Weapon)
		{
			ModeManager->GetCurrentWeaponComponent()->Dash(false);
			return;
		}

		if (SelectedDodge.AnimMontage != nullptr)
		{
			FOnMontageEnded OnMontageEnded;
			OnMontageEnded.BindLambda([this](UAnimMontage* AnimMontage, bool bInterrupted) {

				if (!bInterrupted)
				{
					SetState(ECharacterState::Idle);
				}

				SetCanBeDamaged(true);

				});

			MontagePlay(SelectedDodge);
			SetState(ECharacterState::Dash);
			SetCanBeDamaged(false);
			GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded, SelectedDodge.AnimMontage);
		}
		
	}
}

void ABasePlayer::Reload()
{
	SMOOTH_CHECK(ModeManager);

	auto* CurrentWeaponComponent = ModeManager->GetCurrentWeaponComponent();

	if (auto* const GunActing = Cast<IGunActingManagement>(CurrentWeaponComponent))
	{
		GunActing->Reload();
	}
}

void ABasePlayer::ChangeGunTrigger()
{
	SMOOTH_CHECK(ModeManager);

	auto* CurrentWeaponComponent = ModeManager->GetCurrentWeaponComponent();

	if (auto* const GunActing = Cast<IGunActingManagement>(CurrentWeaponComponent))
	{
		GunActing->ChangeGunTrigger();
	}
}

void ABasePlayer::StartAttack()
{
	SMOOTH_CHECK(ModeManager);

	auto* CurrentWeaponComponent = ModeManager->GetCurrentWeaponComponent();

	if (!CurrentWeaponComponent)
	{
		return;
	}

	if (auto* const ArmingWeapon = CurrentWeaponComponent->GetArmingWeapon())
	{
		Cast<IAttackManagement>(ArmingWeapon)->StartAttack();
	}
}

void ABasePlayer::AttackTick(float DeltaTime, float TotalDuration)
{
	SMOOTH_CHECK(ModeManager);

	auto* CurrentWeaponComponent = ModeManager->GetCurrentWeaponComponent();

	if (!CurrentWeaponComponent)
	{
		return;
	}

	if (auto* const ArmingWeapon = CurrentWeaponComponent->GetArmingWeapon())
	{
		Cast<IAttackManagement>(ArmingWeapon)->AttackTick(DeltaTime, TotalDuration);
	}
}

void ABasePlayer::EndAttack()
{
	SMOOTH_CHECK(ModeManager);

	auto* CurrentWeaponComponent = ModeManager->GetCurrentWeaponComponent();

	if (!CurrentWeaponComponent)
	{
		return;
	}

	if (auto* const ArmingWeapon = CurrentWeaponComponent->GetArmingWeapon())
	{
		Cast<IAttackManagement>(ArmingWeapon)->EndAttack();
	}
}

void ABasePlayer::OnNotifyModeChange()
{
	SMOOTH_CHECK(ModeManager);

	ModeManager->ChangeMode();
}

void ABasePlayer::OnNotifyModeChangeNone()
{
	SMOOTH_CHECK(ModeManager);

	ModeManager->ChangeModeNone();
}

bool ABasePlayer::AddItemToInventory(class AInventoryItem* const WantToAdd)
{
	SMOOTH_CHECK(InventoryManager, false);

	bool Result = InventoryManager->AddItemToInventory(WantToAdd);

	auto* BasePlayerController = Cast<ABasePlayerController>(GetController());

	if (Result && BasePlayerController != nullptr)
	{
		BasePlayerController->GetInventoryUI()->UpdateItems(WantToAdd->GetItemClassification());
	}

	return Result;
}

bool ABasePlayer::SubItemToInventory(AInventoryItem* const WantToAdd, int32 Count)
{
	SMOOTH_CHECK(InventoryManager, false);

	bool Result = InventoryManager->SubItemToInventory(WantToAdd, Count);

	auto* BasePlayerController = Cast<ABasePlayerController>(GetController());

	if (Result && BasePlayerController != nullptr)
	{
		BasePlayerController->GetInventoryUI()->UpdateItems(WantToAdd->GetItemClassification());
	}

	return Result;
}

bool ABasePlayer::SubItemToInventory(EItemClassification InItemClassification, int32 Index, int32 Count)
{
	SMOOTH_CHECK(InventoryManager, false);

	bool Result = InventoryManager->SubItemToInventory(InItemClassification, Index, Count);

	auto* BasePlayerController = Cast<ABasePlayerController>(GetController());

	if (Result && BasePlayerController != nullptr)
	{
		BasePlayerController->GetInventoryUI()->UpdateItems(InItemClassification);
	}

	return Result;
}

void ABasePlayer::ShowGauge()
{
	if (Gauge)
	{
		if (auto* const GaugeWidget = Gauge->GetWidget())
		{
			GaugeWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ABasePlayer::HideGauge()
{
	if (Gauge)
	{
		if (auto* const GaugeWidget = Gauge->GetWidget())
		{
			GaugeWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ABasePlayer::SetGaugePercent(float Percent)
{
	if (Gauge)
	{
		if (auto* const GaugeWidget = Cast<UGaugeUI>(Gauge->GetWidget()))
		{
			GaugeWidget->SetGaugeBarPercent(Percent);
		}
	}
}

UModeManageComponent* ABasePlayer::GetModeManager()
{
	return ModeManager;
}

void ABasePlayer::Zoom(float Alpha, FZoomProperty ZoomProperty)
{
	auto* const PlayerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	
	float BaseFieldOfView = ZoomProperty.BaseFieldOfView;
	float ZoomRate = ZoomProperty.ZoomRate;

	PlayerAnim->SetZoomAlpha(Alpha);
	State->SetZoomAlpha(Alpha);
	Camera->SetFieldOfView(FMath::Lerp(BaseFieldOfView, BaseFieldOfView / ZoomRate, Alpha));
}

UCameraComponent* ABasePlayer::GetCamera()
{
	return Camera;
}

void ABasePlayer::ChangeWeaponMode(EWeaponMode WeaponMode)
{
	switch (WeaponMode)
	{
	case EWeaponMode::None:
	{
		SpringArm->TargetArmLength = 400.0f;
		SpringArm->SocketOffset = FVector(0.f, 0.f, 0.f);
		SpringArm->TargetOffset = FVector(0.f, 0.f, 80.f);
		break;
	}
	case EWeaponMode::Soldier:
	{
		SpringArm->TargetArmLength = 250.0f;
		SpringArm->SocketOffset = FVector(0.f, 70.f, 0.f);
		SpringArm->TargetOffset = FVector(0.f, 0.f, 100.f);
		break;
	}
	case EWeaponMode::Weapon:
	{
		SpringArm->TargetArmLength = 300.0f;
		SpringArm->SocketOffset = FVector(0.f, -70.f, 0.f);
		SpringArm->TargetOffset = FVector(0.f, 0.f, 100.f);
		break;
	}
	}
}

void ABasePlayer::OnChangeModeTimeline(float Alpha)
{
	static EWeaponMode BeforeWeaponMode;
	static EWeaponMode AfterWeaponMode;
	
	if (bChangeModeStart)	//true when ChangeMode(), MontagePlay
	{
		BeforeWeaponMode = ModeManager->GetWeaponMode();
		AfterWeaponMode = ModeManager->GetAfterChangeMode(BeforeWeaponMode);
	}

	bChangeModeStart = false;

	FChangeModeCameraSetting* BeforeCameraSetting = CameraSettingMap.Find(BeforeWeaponMode);
	FChangeModeCameraSetting* AfterCameraSetting = CameraSettingMap.Find(AfterWeaponMode);

	SpringArm->TargetArmLength = FMath::Lerp(BeforeCameraSetting->TargetArmLength, AfterCameraSetting->TargetArmLength, Alpha);
	SpringArm->SocketOffset = FMath::Lerp(BeforeCameraSetting->SocketOffset, AfterCameraSetting->SocketOffset, Alpha);
	SpringArm->TargetOffset = FMath::Lerp(BeforeCameraSetting->TargetOffset, AfterCameraSetting->TargetOffset, Alpha);
}

void ABasePlayer::OnChangeModeNoneTimeline(float Alpha)
{
	static EWeaponMode BeforeWeaponMode;
	static EWeaponMode AfterWeaponMode;

	if (bChangeModeStart)	//true when ChangeMode(), MontagePlay
	{
		BeforeWeaponMode = ModeManager->GetWeaponMode();
		AfterWeaponMode = EWeaponMode::None;
	}

	bChangeModeStart = false;

	FChangeModeCameraSetting* BeforeCameraSetting = CameraSettingMap.Find(BeforeWeaponMode);
	FChangeModeCameraSetting* AfterCameraSetting = CameraSettingMap.Find(AfterWeaponMode);

	SpringArm->TargetArmLength = FMath::Lerp(BeforeCameraSetting->TargetArmLength, AfterCameraSetting->TargetArmLength, Alpha);
	SpringArm->SocketOffset = FMath::Lerp(BeforeCameraSetting->SocketOffset, AfterCameraSetting->SocketOffset, Alpha);
	SpringArm->TargetOffset = FMath::Lerp(BeforeCameraSetting->TargetOffset, AfterCameraSetting->TargetOffset, Alpha);
}
