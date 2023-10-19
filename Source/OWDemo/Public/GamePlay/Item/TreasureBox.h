// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "WorldItem.h"
#include "TreasureBox.generated.h"

UCLASS()
class OWDEMO_API ATreasureBox : public AWorldItem
{
	GENERATED_BODY()
	
public:
	ATreasureBox();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) final;

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AItem> SpawnItemClass;

	UPROPERTY(EditAnywhere)
		float ParticleStartHeight;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = true))
		class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = true))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Particle, meta = (AllowPrivateAccess = true))
		class UNiagaraSystem* Particle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundBase* Sound;
};
