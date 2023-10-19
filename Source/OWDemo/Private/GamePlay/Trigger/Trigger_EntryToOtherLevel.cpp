// If you want to use this project, mail to ggoggal@gmail.com

#include "Trigger_EntryToOtherLevel.h"
#include "Components/BoxComponent.h"
#include "BasePlayer.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSetting.h"
#include "PlayerUI.h"
#include "BasePlayerController.h"

ATrigger_EntryToOtherLevel::ATrigger_EntryToOtherLevel()
{
	CreateDefaultSubobjectAuto(TriggerBox);
	CreateDefaultSubobjectAuto(Mesh);

	if (Mesh)
	{
		SetRootComponent(Mesh);
	}

	if (TriggerBox)
	{
		TriggerBox->SetCollisionProfileName(OWCollision::PresetName_PlayerTrigger);
		TriggerBox->SetBoxExtent(FVector(200.f, 50.f, 200.f));
		TriggerBox->SetupAttachment(GetRootComponent());
	}

	OverlapMessage = "Press 'F' To Next Level!";
}

void ATrigger_EntryToOtherLevel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InputComponent = NewObject<UInputComponent>(this);
	InputComponent->RegisterComponent();
	InputComponent->BindAction("Interact", IE_Pressed, this, &ThisClass::Interact);
}

void ATrigger_EntryToOtherLevel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATrigger_EntryToOtherLevel::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	Player = Cast<ABasePlayer>(OtherActor);

	if (Player.Get())
	{
		ABasePlayerController* PlayerController = Cast<ABasePlayerController>(Player->GetController());
		
		if (PlayerController)
		{
			EnableInput(PlayerController);
			PlayerController->GetPlayerUI()->SetInfoToPlayer(OverlapMessage);
		}
		else
		{
			LOG(Warning, TEXT("Player Exist, But Don't Possessed By PlayerController"));
		}
	}
}

void ATrigger_EntryToOtherLevel::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (Player == OtherActor)
	{
		ABasePlayerController* PlayerController = Cast<ABasePlayerController>(Player->GetController());

		if (PlayerController)
		{
			DisableInput(PlayerController);
			PlayerController->GetPlayerUI()->SetInfoToPlayer("");
		}

		Player = nullptr;
	}
}

void ATrigger_EntryToOtherLevel::Interact()
{
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();

	if (MapToLoad.IsValid())
	{
		LevelGenerator_Handle = StreamableManager.RequestAsyncLoad(MapToLoad, FStreamableDelegate::CreateUObject(this, &ThisClass::MapLoadComplete));
	}
}

void ATrigger_EntryToOtherLevel::MapLoadComplete()
{
	UWorld* NextLevel = Cast<UWorld>(LevelGenerator_Handle->GetLoadedAsset());

	if (NextLevel)
	{
		UGameplayStatics::OpenLevel(this, NextLevel->GetFName());
	}
	else
	{
		LOG(Warning, TEXT("Level Did Not Loaded!, Check FSoftObjectPtr MapToLoad!!"));
	}
}
