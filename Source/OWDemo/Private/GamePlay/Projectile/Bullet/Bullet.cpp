// If you want to use this project, mail to ggoggal@gmail.com


#include "Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "OWDemoSetting.h"
#include "BaseCharacter.h"
#include "BulletTracerDataAsset.h"

ABullet::ABullet()
{
	//InitComponents
	CreateDefaultSubobjectAuto(Collision);
	CreateDefaultSubobjectAuto(Mesh);
	CreateDefaultSubobjectAuto(Tracer);

	//Attach
	SetRootComponent(Collision);
	Mesh->SetupAttachment(GetRootComponent());
	Tracer->SetupAttachment(GetRootComponent());

	//Setup
	Collision->SetSphereRadius(4.f);
	Collision->SetCollisionProfileName(OWCollision::PresetName_Bullet);

	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	static ConstructorHelpers::FObjectFinder<UBulletTracerDataAsset> BULLET_TRACER(TEXT("BulletTracerDataAsset'/Game/Datas/DataAsset/BulletTracer/DA_BulletTracer.DA_BulletTracer'"));

	if (BULLET_TRACER.Succeeded())
	{
		BulletTracerData = BULLET_TRACER.Object;
	}
}

void ABullet::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetLifeSpan(2.f);
}

void ABullet::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (Other == GetInstigator())
	{
		return;
	}

	if (auto* const BaseCharacter = Cast<ABaseCharacter>(Other))
	{
		FPointDamageEvent PointDamageEvent;

		BaseCharacter->TakeDamage(GetDamage(), PointDamageEvent, GetInstigatorController(), this);
	}

	Destroy();
}

void ABullet::Initialize(float InSpeed, float InDamage, UStaticMesh* const InMesh, ESoldierWeaponType InWeaponType)
{
	Speed = InSpeed;
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	
	SetDamage(InDamage);

	SetTracer(InWeaponType);

	if (Mesh->SetStaticMesh(InMesh) == false)
	{
		LOG(Warning, TEXT("Bullet's Mesh Is Not Setted!!"));
	}
}

void ABullet::SetTracer(ESoldierWeaponType WeaponType)
{
	SMOOTH_CHECK(BulletTracerData);

	if (auto* const BulletTracer = BulletTracerData->GetTracerParticle(WeaponType))
	{
		Tracer->SetTemplate(BulletTracer);
	}
	else
	{
		LOG(Warning, TEXT("No Tracer In Bullet"));
	}
}