// If you want to use this project, mail to ggoggal@gmail.com

#include "BaseEnemy.h"
#include "BaseEnemyController.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GaugeUI.h"
#include "Kismet/GameplayStatics.h"

ABaseEnemy::ABaseEnemy()
{
	CreateDefaultSubobjectAuto(WeaponManager);
	CreateDefaultSubobjectAuto(HPGauge);

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionProfileName(OWCollision::PresetName_Enemy);
	}

	if (GetMesh())
	{
		GetMesh()->SetCollisionProfileName(OWCollision::PresetName_NoCollision);
	}

	if (HPGauge)
	{
		HPGauge->SetupAttachment(GetRootComponent());

		static ConstructorHelpers::FClassFinder<UUserWidget> GAUGE_WIDGET
		(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_GaugeUI.BP_GaugeUI_C'"));

		if (GAUGE_WIDGET.Succeeded())
		{
			HPGauge->SetWidgetClass(GAUGE_WIDGET.Class);
			HPGauge->SetWidgetSpace(EWidgetSpace::Screen);
			HPGauge->SetDrawSize(FVector2D(200.f, 20.f));
		}
	}

	ShowHPGaugeTime = 3.f;
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (auto* const GaugeUI = Cast<UGaugeUI>(HPGauge->GetWidget()))
	{
		GaugeUI->SetBarColor(FLinearColor::Red);
		HideHPGauge();
	}

	SetMaxHP(2000.f, true);
}

void ABaseEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	HPGauge->SetRelativeLocation(FVector(0.f, 0.f, CapsuleHalfHeight + 30.f));
}

float ABaseEnemy::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (IsValid(this))
	{
		if (auto* const EnemyController = Cast<ABaseEnemyController>(GetController()))
		{
			if (EnemyController->GetTarget() == nullptr)
			{
				auto* const Target = Cast<ABaseCharacter>(EventInstigator->GetPawn());
				EnemyController->SetTarget(Target);
				EnemyController->LookTarget(true);
			}
		}

		ShowHPGauge();

		FLatentActionInfo LatentActionInfo(1, GetActorGuid().D, TEXT("HideHPGauge"), this);
		UKismetSystemLibrary::RetriggerableDelay(this, ShowHPGaugeTime, LatentActionInfo);
	}

	return Damage;
}

float ABaseEnemy::AddCurrentHP(float AddHP)
{
	float CurrentHP = Super::AddCurrentHP(AddHP);

	if (auto* const GaugeUI = Cast<UGaugeUI>(HPGauge->GetWidget()))
	{
		FStat MaxHP = GetStat(EStat::MaxHP);

		GaugeUI->SetGaugeBarPercent(CurrentHP / MaxHP.Value);
	}

	return CurrentHP;
}

void ABaseEnemy::ShowHPGauge()
{
	HPGauge->GetWidget()->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void ABaseEnemy::HideHPGauge()
{
	HPGauge->GetWidget()->SetVisibility(ESlateVisibility::Collapsed);
}