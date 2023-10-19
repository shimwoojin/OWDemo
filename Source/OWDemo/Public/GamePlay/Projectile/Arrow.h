// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "DamageableProjectile.h"
#include "Arrow.generated.h"

UCLASS()
class OWDEMO_API AArrow : public ADamageableProjectile
{
	GENERATED_BODY()

public:
	AArrow();

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) final;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) final;

public:
	void Initialize(float InitialSpeed, float InGravityScale, float InDamage, FRotator AimRotation);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = true))
		class USkeletalMeshComponent* ArrowMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = true))
		class USphereComponent* SphereCollision;

	float DamageAfterDeleteTime;
};
