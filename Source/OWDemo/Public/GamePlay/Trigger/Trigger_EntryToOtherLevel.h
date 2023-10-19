// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "GameFramework/Actor.h"
#include "Trigger_EntryToOtherLevel.generated.h"

UCLASS()
class OWDEMO_API ATrigger_EntryToOtherLevel : public AActor
{
	GENERATED_BODY()
	
public:	
	ATrigger_EntryToOtherLevel();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) final;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) final;

private:
	void Interact();

	UFUNCTION()
		void MapLoadComplete();

public:
	UPROPERTY(EditAnywhere)
		FSoftObjectPath MapToLoad;

	UPROPERTY(EditAnywhere)
		FString OverlapMessage;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = true))
		class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = true))
		class UStaticMeshComponent* Mesh;

	TWeakObjectPtr<class ABasePlayer> Player;

	TSharedPtr<struct FStreamableHandle> LevelGenerator_Handle;
};
