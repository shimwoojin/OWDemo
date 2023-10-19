// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class OWDEMO_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	FString GetItemName() { return ItemName; }
	void SetItemName(FString InItemName) { ItemName = InItemName; }

	virtual FString GetItemInfo() { return ""; }

private:
	FString ItemName;
};
