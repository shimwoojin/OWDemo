// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "GameFramework/PlayerController.h"
#include "ActingManagement.h"
#include "GunActingManagement.h"
#include "ModeManagement.h"
#include "ViewManagement.h"
#include "CharacterManagement.h"
#include "BasePlayerController.generated.h"

UCLASS()
class OWDEMO_API ABasePlayerController : 
	public APlayerController,
	public IActingManagement,
	public IModeManagement,
	public IViewManagement,
	public ICharacterManagement,
	public IGunActingManagement
{
	GENERATED_BODY()
	
public:
	ABasePlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* MyPawn) override;
	virtual void OnUnPossess() override;

public:
	UObject* GetInventoryItemData() { return ItemDataFromInventory.Get(); }
	void SetInventoryItemData(UObject* Object);

	class UInventoryUI* GetInventoryUI() { return InventoryUI; }
	class UItemEquipmentUI* GetItemEquipmentUI() { return ItemEquipmentUI; }
	class UPlayerUI* GetPlayerUI() { return PlayerUI; }

private:

	//BindAxis
	virtual void BindAxis();

#pragma region AXIS_DEFAULT
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
#pragma endregion
	
	//BindAction
	virtual void BindAction();

#pragma region ACTION_DEFAULT
	void Jump(bool bPressed);
	void Crouch(bool bPressed);
#pragma endregion

#pragma region VIEW
	virtual void ToggleCameraAngle() final;
#pragma endregion

#pragma region ACTING
	virtual void Dash(bool bPressed) final;
	virtual void Sprint(bool bPressed) final;
	virtual void Swap(int32 Number) final;
	virtual void Attack(bool bPressed) final;
	virtual void AttackAssist(bool bPressed) final;
	virtual void ActivateSkill(int32 Number, bool bPressed) final;
#pragma endregion

	void LMouseButton(bool bPressed);

#pragma region GUN_ACTING
	virtual void Reload() final;
	virtual void ChangeGunTrigger() final;
#pragma endregion

#pragma region MODE
	virtual void ChangeMode() final;
	virtual void ChangeModeNone() final;
#pragma endregion

	void ToggleVisiblityInventoryUI();
	void ToggleVisiblityItemEquipmentUI();

	void SetupUI();

private:
	class ABasePlayer* Player;

	FVector InputVector;

private:
	UPROPERTY()
		class UInventoryUI* InventoryUI;

	UPROPERTY()
		class UItemEquipmentUI* ItemEquipmentUI;

	UPROPERTY()
		class UPlayerUI* PlayerUI;

	UPROPERTY()
		class UEquipedItemUI* MouseGrabbedUI;

	TSet<UUserWidget*> VisibleUIs;

	UPROPERTY()
		TWeakObjectPtr<UObject> ItemDataFromInventory;
};
