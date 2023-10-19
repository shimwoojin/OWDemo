// If you want to use this project, mail to ggoggal@gmail.com

#include "InventoryItem.h"
#include "BasePlayer.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.h"
#include "PlayerUI.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

USoundBase* AInventoryItem::PickUpSound = nullptr;

AInventoryItem::AInventoryItem()
{
	bCanMerge = false;
	bOwnedByPlayer = false;
	ItemClassification = EItemClassification::None;

	ConstructorHelpers::FObjectFinder<USoundCue> PICK_UP_SOUND
	(TEXT("SoundCue'/Game/Assets/InterfaceAndItemSounds/Cues/Coins_01_wav_Cue.Coins_01_wav_Cue'"));
	if (PICK_UP_SOUND.Succeeded())
	{
		PickUpSound = PICK_UP_SOUND.Object;
	}
}

void AInventoryItem::BeginPlay()
{
	Super::BeginPlay();

	InputComponent = NewObject<UInputComponent>(this);
	InputComponent->RegisterComponent();

	if (InputComponent)
	{
		InputComponent->BindAction("Interact", IE_Pressed, this, &AInventoryItem::PickUp);
	}

	UniqueID = GetItemName() + "_" + GetActorGuid().ToString();
}

void AInventoryItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!IsOwnedByPlayer())
	{
		if (auto* const BasePlayer = Cast<ABasePlayer>(OtherActor))
		{
			Player = BasePlayer;
			auto* const PlayerController = Cast<ABasePlayerController>(Player->GetController());
			SMOOTH_CHECK(PlayerController);
			EnableInput(PlayerController);

			if (auto* PlayerUI = PlayerController->GetPlayerUI())
			{
				PlayerUI->SetInfoToPlayer(FString::Printf(TEXT("Press 'F' To Get '%s'"), *GetItemName()));
			}
		}
	}
}

void AInventoryItem::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (!IsOwnedByPlayer())
	{
		if (auto* const BasePlayer = Cast<ABasePlayer>(OtherActor))
		{
			auto* const PlayerController = Cast<ABasePlayerController>(BasePlayer->GetController());
			SMOOTH_CHECK(PlayerController);
			DisableInput(PlayerController);
			Player = nullptr;

			if (auto* PlayerUI = PlayerController->GetPlayerUI())
			{
				PlayerUI->SetInfoToPlayer("");
			}
		}
	}
}

void AInventoryItem::PickUp()
{
	if (AddItemToInventory(this))
	{
		if (Player.Get())
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PickUpSound, GetActorLocation(), GetActorRotation());
			Player->PickUpItem_MontagePlay();
		}

		Destroy();
	}
}

bool AInventoryItem::AddItemToInventory(AInventoryItem* const WantToAdd)
{
	if (auto* const InventoryManaged = Cast<IInventoryManagement>(Player.Get()))
	{
		return InventoryManaged->AddItemToInventory(WantToAdd);
	}
	return false;
}

FString AInventoryItem::GetInventoryItemUniqueID()
{
	return UniqueID;
}

FString AInventoryItem::SetInventoryItemUniqueID(FString InUniqueID)
{
	UniqueID = InUniqueID;
	return UniqueID;
}
