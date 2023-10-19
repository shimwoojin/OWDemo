// If you want to use this project, mail to ggoggal@gmail.com


#include "Arrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseCharacter.h"

AArrow::AArrow()
{
	CreateDefaultSubobjectAuto(SphereCollision);
	if (SphereCollision)
	{
		SetRootComponent(SphereCollision);
		SphereCollision->SetSphereRadius(2.f);
		SphereCollision->SetCollisionProfileName(OWCollision::PresetName_Arrow);
		SphereCollision->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));
	}

	CreateDefaultSubobjectAuto(ArrowMesh);
	if (ArrowMesh)
	{
		ArrowMesh->SetupAttachment(GetRootComponent());
		ArrowMesh->SetRelativeLocationAndRotation(FVector(-2.5f, 0.f, 3.f), FRotator(-90.f, 0.f, 0.f));
		ArrowMesh->SetCollisionProfileName(OWCollision::PresetName_NoCollision);

		static ConstructorHelpers::FObjectFinder<USkeletalMesh> ARROW
		(TEXT("SkeletalMesh'/Game/Assets/Weapons/BowAndArrow/Metal_Bow_and_Arrow_Metal_Arrow.Metal_Bow_and_Arrow_Metal_Arrow'"));
		if (ARROW.Succeeded())
		{
			ArrowMesh->SetSkeletalMesh(ARROW.Object);
		}
	}

	if (ProjectileMovement)
	{
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->ProjectileGravityScale = 0.f;
		ProjectileMovement->bSimulationEnabled = true;
		ProjectileMovement->Velocity = FVector(0.f, 0.f, 0.f);
	}

	DamageAfterDeleteTime = 30.f;
}

void AArrow::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	SetLifeSpan(DamageAfterDeleteTime);
}

void AArrow::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	APawn* LocalInstigator = GetInstigator();

	if (LocalInstigator)
	{
		if (OtherActor != LocalInstigator && Cast<ABaseCharacter>(OtherActor))
		{
			FDamageEvent DamageEvent;
			OtherActor->TakeDamage(GetDamage(), DamageEvent, LocalInstigator->GetController(), this);
			ProjectileMovement->bSimulationEnabled = false;
			FAttachmentTransformRules AttachRule(EAttachmentRule::KeepWorld, false);
			AttachToActor(OtherActor, AttachRule);
			SetLifeSpan(DamageAfterDeleteTime);
		}
	}
}

void AArrow::Initialize(float InitialSpeed, float InGravityScale, float InDamage, FRotator AimRotation)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = AimRotation.Vector() * InitialSpeed;
		ProjectileMovement->MaxSpeed = InitialSpeed;
		ProjectileMovement->ProjectileGravityScale = InGravityScale;
		SetDamage(InDamage);
	}
}
