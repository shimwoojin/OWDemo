// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "DamageableProjectile.h"
#include "ElementalBall.generated.h"

UCLASS(Abstract)
class OWDEMO_API AElementalBall : public ADamageableProjectile
{
	GENERATED_BODY()
	
public:
	AElementalBall();

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) final;

public:
	void Initialize(float InitialSpeed, float InDamage, FRotator AimRotation);

	void EnableOverlapEvents();
	void DisableOverlapEvents();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = true))
		class USphereComponent* SphereCollision;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* ElementalBallParticle;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* ElementalBallHitEffect;
};
