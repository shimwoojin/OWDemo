// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "OWEnums.h"
#include "DamageableProjectile.h"
#include "BulletFactory.h"
#include "Bullet.generated.h"

UCLASS()
class OWDEMO_API ABullet : public ADamageableProjectile
{
	GENERATED_BODY()

	friend ABullet* UBulletFactory::BuildBullet();

public:
	ABullet();

	virtual void PostInitializeComponents() override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit);

private:
	void Initialize(float InSpeed, float InDamage, class UStaticMesh* const InMesh, ESoldierWeaponType InWeaponType);
	void SetTracer(ESoldierWeaponType WeaponType);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = true))
		class USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = true))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* Tracer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = true))
		class UBulletTracerDataAsset* BulletTracerData;

	float Speed;
};
