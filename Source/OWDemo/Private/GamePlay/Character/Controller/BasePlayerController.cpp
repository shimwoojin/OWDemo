// If you want to use this project, mail to ggoggal@gmail.com


#include "BasePlayerController.h"
#include "BasePlayer.h"
#include "InventoryUI.h"
#include "ItemUIData.h"
#include "ItemEquipmentUI.h"
#include "PlayerUI.h"
#include "EquipedItemUI.h"
#include "InventoryItem.h"
#include "Kismet/GameplayStatics.h"

ABasePlayerController::ABasePlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetupUI();
}

void ABasePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Player)
	{
		InputVector.Normalize();

		Player->AddMovementInput(InputVector);
		InputVector = FVector::ZeroVector;

		if (bShowMouseCursor && GetInventoryItemData())
		{
			UItemUIData* ItemUIData = Cast<UItemUIData>(GetInventoryItemData());

			if (ItemUIData)
			{
				FVector2D MousePosition;
				GetMousePosition(MousePosition.X, MousePosition.Y);
				MouseGrabbedUI->SetPositionInViewport(MousePosition);
			}
		}

		if (PlayerUI->IsNormalSkillUIVisible() || PlayerUI->IsUltSkillUIVisible())
		{
			PlayerUI->UpdateSkillCoolTime();
		}
	}
	
}

void ABasePlayerController::OnPossess(APawn* MyPawn)
{
	Super::OnPossess(MyPawn);

	Player = Cast<ABasePlayer>(GetPawn());
	check(Player);
}

void ABasePlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	Player = nullptr;
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	BindAxis();
	BindAction();
}

void ABasePlayerController::BindAxis()
{
	InputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ThisClass::MoveRight);

	InputComponent->BindAxis("Turn", this, &ThisClass::Turn);
	InputComponent->BindAxis("LookUp", this, &ThisClass::LookUp);
}

void ABasePlayerController::MoveForward(float Value)
{
	FRotator Rotation = GetControlRotation();
	InputVector += FRotationMatrix(Rotation).GetUnitAxis(EAxis::X) * Value;
}

void ABasePlayerController::MoveRight(float Value)
{
	FRotator Rotation = GetControlRotation();
	InputVector += FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y) * Value;
}

void ABasePlayerController::Turn(float Value)
{
	AddYawInput(Value);
}

void ABasePlayerController::LookUp(float Value)
{
	AddPitchInput(Value);
}

void ABasePlayerController::BindAction()
{
	InputComponent->BindAction("ToggleCameraAngle", IE_Pressed, this, &ThisClass::ToggleCameraAngle);

	InputComponent->BindAction<TDelegate<void(bool)>>("Jump", IE_Pressed, this, &ThisClass::Jump, true);
	InputComponent->BindAction<TDelegate<void(bool)>>("Jump", IE_Released, this, &ThisClass::Jump, false);

	InputComponent->BindAction<TDelegate<void(bool)>>("Dash", IE_Pressed, this, &ThisClass::Dash, true);
	InputComponent->BindAction<TDelegate<void(bool)>>("Dash", IE_Released, this, &ThisClass::Dash, false);

	InputComponent->BindAction<TDelegate<void(bool)>>("Crouch", IE_Pressed, this, &ThisClass::Crouch, true);
	InputComponent->BindAction<TDelegate<void(bool)>>("Crouch", IE_Released, this, &ThisClass::Crouch, false);

	InputComponent->BindAction<TDelegate<void(int32)>>("Swap1", IE_Pressed, this, &ThisClass::Swap, 0);
	InputComponent->BindAction<TDelegate<void(int32)>>("Swap2", IE_Pressed, this, &ThisClass::Swap, 1);
	InputComponent->BindAction<TDelegate<void(int32)>>("Swap3", IE_Pressed, this, &ThisClass::Swap, 2);
	InputComponent->BindAction<TDelegate<void(int32)>>("Swap4", IE_Pressed, this, &ThisClass::Swap, 3);

	InputComponent->BindAction<TDelegate<void(bool)>>("LMouseButton", IE_Pressed, this, &ThisClass::LMouseButton, true);
	InputComponent->BindAction<TDelegate<void(bool)>>("LMouseButton", IE_Released, this, &ThisClass::LMouseButton, false);
	InputComponent->BindAction<TDelegate<void(bool)>>("AttackAssist", IE_Pressed, this, &ThisClass::AttackAssist, true);
	InputComponent->BindAction<TDelegate<void(bool)>>("AttackAssist", IE_Released, this, &ThisClass::AttackAssist, false);

	InputComponent->BindAction<TDelegate<void(int32, bool)>>("Skill1", IE_Pressed, this, &ThisClass::ActivateSkill, 0, true);
	InputComponent->BindAction<TDelegate<void(int32, bool)>>("Skill1", IE_Released, this, &ThisClass::ActivateSkill, 0, false);
	InputComponent->BindAction<TDelegate<void(int32, bool)>>("Skill2", IE_Pressed, this, &ThisClass::ActivateSkill, 1, true);
	InputComponent->BindAction<TDelegate<void(int32, bool)>>("Skill2", IE_Released, this, &ThisClass::ActivateSkill, 1, false);

	InputComponent->BindAction("Reload", IE_Pressed, this, &ThisClass::Reload);
	InputComponent->BindAction("ChangeGunTrigger", IE_Pressed, this, &ThisClass::ChangeGunTrigger);

	InputComponent->BindAction("ChangeMode", IE_Pressed, this, &ThisClass::ChangeMode);
	InputComponent->BindAction("ChangeModeNone", IE_Pressed, this, &ThisClass::ChangeModeNone);

	InputComponent->BindAction("Inventory", IE_Pressed, this, &ThisClass::ToggleVisiblityInventoryUI);
	InputComponent->BindAction("ItemEquipment", IE_Pressed, this, &ThisClass::ToggleVisiblityItemEquipmentUI);
}

void ABasePlayerController::ToggleCameraAngle()
{
	if (Player)
	{
		Player->ToggleCameraAngle();
	}
}

void ABasePlayerController::Jump(bool bPressed)
{
	if (Player)
	{
		if (bPressed)
		{
			Player->Jump();
		}
		else
		{
			Player->StopJumping();
		}
	}
}

void ABasePlayerController::Dash(bool bPressed)
{
	SMOOTH_CHECK(Player);
	
	Player->Dash(bPressed);
}

void ABasePlayerController::Sprint(bool bPressed)
{
	SMOOTH_CHECK(Player);

	Player->Sprint(bPressed);
}

void ABasePlayerController::Crouch(bool bPressed)
{
	SMOOTH_CHECK(Player);

	if (bPressed)
	{
		Player->Crouch();
	}
	else
	{
		Player->UnCrouch();
	}
}

void ABasePlayerController::Swap(int32 Number)
{
	SMOOTH_CHECK(Player);

	Player->Swap(Number);
}

void ABasePlayerController::Attack(bool bPressed)
{
	SMOOTH_CHECK(Player);

	Player->Attack(bPressed);
}

void ABasePlayerController::AttackAssist(bool bPressed)
{
	SMOOTH_CHECK(Player);

	Player->AttackAssist(bPressed);
}

void ABasePlayerController::ActivateSkill(int32 Number, bool bPressed)
{
	SMOOTH_CHECK(Player);

	Player->ActivateSkill(Number, bPressed);
}

void ABasePlayerController::LMouseButton(bool bPressed)
{
	if (ItemDataFromInventory.Get())
	{
		if (!bPressed)
		{
			auto* ItemUIData = Cast<UItemUIData>(ItemDataFromInventory);

			if (ItemUIData)
			{
				if (ItemUIData->DataFrom == EDataFrom::InventoryUI)
				{
					auto* InventoryItem = Cast<AInventoryItem>(ItemUIData->SpawnItemClass->GetDefaultObject());

					if (InventoryItem)
					{
						Player->SubItemToInventory(InventoryItem->GetItemClassification(), ItemUIData->DataFromIndex, 1);
						ItemDataFromInventory = nullptr;
						MouseGrabbedUI->SetVisibility(ESlateVisibility::Collapsed);

						if (UGameplayStatics::DoesSaveGameExist(ItemUIData->SaveSlotName, ItemUIData->SaveSlotIndex))
						{
							UGameplayStatics::DeleteGameInSlot(ItemUIData->SaveSlotName, ItemUIData->SaveSlotIndex);
						}
					}
				}
			}
		}
	}
	else
	{
		Attack(bPressed);
	}
}

void ABasePlayerController::Reload()
{
	SMOOTH_CHECK(Player);

	Player->Reload();
}

void ABasePlayerController::ChangeGunTrigger()
{
	SMOOTH_CHECK(Player);

	Player->ChangeGunTrigger();
}

void ABasePlayerController::ChangeMode()
{
	SMOOTH_CHECK(Player);

	Player->ChangeMode();
}

void ABasePlayerController::ChangeModeNone()
{
	SMOOTH_CHECK(Player);

	Player->ChangeModeNone();
}

void ABasePlayerController::ToggleVisiblityInventoryUI()
{
	FInputModeGameAndUI GameAndUI;
	FInputModeGameOnly GameOnly;

	if (InventoryUI && Player)
	{
		switch (InventoryUI->GetVisibility())
		{
		case ESlateVisibility::SelfHitTestInvisible:
		{
			InventoryUI->SetVisibility(ESlateVisibility::Collapsed);
			VisibleUIs.Remove(InventoryUI);
			
			if (VisibleUIs.Num() <= 0)
			{
				bShowMouseCursor = false;
				SetInputMode(GameOnly);
				SetInventoryItemData(nullptr);
			}

			break;
		}
		case ESlateVisibility::Collapsed:
		{
			InventoryUI->UpdateItems(EItemClassification::Weapon);
			InventoryUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			bShowMouseCursor = true;
			VisibleUIs.Add(InventoryUI);
			SetInputMode(GameAndUI);
			break;
		}
		}
	}
}

void ABasePlayerController::ToggleVisiblityItemEquipmentUI()
{
	FInputModeGameAndUI GameAndUI;
	FInputModeGameOnly GameOnly;

	if (ItemEquipmentUI && Player)
	{
		switch (ItemEquipmentUI->GetVisibility())
		{
		case ESlateVisibility::SelfHitTestInvisible:
		{
			ItemEquipmentUI->SetVisibility(ESlateVisibility::Collapsed);
			VisibleUIs.Remove(ItemEquipmentUI);
			
			if (VisibleUIs.Num() <= 0)
			{
				bShowMouseCursor = false;
				SetInputMode(GameOnly);
				SetInventoryItemData(nullptr);
			}

			break;
		}
		case ESlateVisibility::Collapsed:
		{
			ItemEquipmentUI->UpdateItems();
			ItemEquipmentUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			bShowMouseCursor = true;
			VisibleUIs.Add(ItemEquipmentUI);
			SetInputMode(GameAndUI);
			break;
		}
		}
	}
}

void ABasePlayerController::SetupUI()
{
	UClass* InventoryUIClass =
		LoadClass<UInventoryUI>(this, TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_InventoryUI.BP_InventoryUI_C'"));

	if (InventoryUIClass)
	{
		InventoryUI = Cast<UInventoryUI>(CreateWidget(this, InventoryUIClass));

		if (InventoryUI)
		{
			InventoryUI->AddToViewport(1);
			InventoryUI->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	UClass* ItempEquipmentUIClass =
		LoadClass<UItemEquipmentUI>(this, TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_ItemEquipmentUI.BP_ItemEquipmentUI_C'"));

	if (ItempEquipmentUIClass)
	{
		ItemEquipmentUI = Cast<UItemEquipmentUI>(CreateWidget(this, ItempEquipmentUIClass));

		if (ItemEquipmentUI)
		{
			ItemEquipmentUI->AddToViewport(1);
			ItemEquipmentUI->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	UClass* PlayerUIClass =
		LoadClass<UPlayerUI>(this, TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_PlayerUI.BP_PlayerUI_C'"));

	if (PlayerUIClass)
	{
		PlayerUI = Cast<UPlayerUI>(CreateWidget(this, PlayerUIClass));

		if (PlayerUI)
		{
			PlayerUI->AddToViewport();
			PlayerUI->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}

	UClass* EquipedItemUIClass =
		LoadClass<UEquipedItemUI>(this, TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_EquipedItemUI.BP_EquipedItemUI_C'"));

	if (EquipedItemUIClass)
	{
		MouseGrabbedUI = Cast<UEquipedItemUI>(CreateWidget(this, EquipedItemUIClass));

		if (MouseGrabbedUI)
		{
			MouseGrabbedUI->AddToViewport(2);
			MouseGrabbedUI->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ABasePlayerController::SetInventoryItemData(UObject* Object)
{
	ItemDataFromInventory = Object;

	if (auto* const ItemUIData = Cast<UItemUIData>(Object))
	{
		MouseGrabbedUI->SetVisibility(ESlateVisibility::HitTestInvisible);

		MouseGrabbedUI->SetItemTexture(ItemUIData);
		MouseGrabbedUI->SetItemName(ItemUIData);
	}
	else
	{
		MouseGrabbedUI->SetVisibility(ESlateVisibility::Collapsed);

		UItemUIData* Temp = NewObject<UItemUIData>(this);

		MouseGrabbedUI->SetItemTexture(Temp);
		MouseGrabbedUI->SetItemName(Temp);
	}
}