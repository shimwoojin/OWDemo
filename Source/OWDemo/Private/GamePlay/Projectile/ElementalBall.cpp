// If you want to use this project, mail to ggoggal@gmail.com

#include "ElementalBall.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BaseCharacter.h"
#include "BaseWeapon.h"

AElementalBall::AElementalBall()
{
	CreateDefaultSubobjectAuto(SphereCollision);
	if (SphereCollision)
	{
		SetRootComponent(SphereCollision);
		SphereCollision->SetCollisionProfileName(OWCollision::PresetName_Magic);
		DisableOverlapEvents();
	}

	CreateDefaultSubobjectAuto(ElementalBallParticle);
	if (ElementalBallParticle)
	{
		ElementalBallParticle->SetupAttachment(GetRootComponent());
	}

	if (ProjectileMovement)
	{
		ProjectileMovement->ProjectileGravityScale = 0.f;
		ProjectileMovement->bSimulationEnabled = true;
		ProjectileMovement->Velocity = FVector(0.f, 0.f, 0.f);
	}
}

void AElementalBall::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (auto* const BaseWeapon = Cast<ABaseWeapon>(OtherActor))
	{
		if (BaseWeapon->GetOwner() == GetInstigator())
		{
			return;
		}
	}

	if (OtherActor != GetInstigator())
	{
		if (Cast<ABaseCharacter>(OtherActor))
		{
			FDamageEvent DamageEvent;
			OtherActor->TakeDamage(GetDamage(), DamageEvent, GetInstigator()->GetController(), this);
		}

		if (ElementalBallHitEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ElementalBallHitEffect, GetActorTransform());
		}

		Destroy();
	}
}

void AElementalBall::Initialize(float InitialSpeed, float InDamage, FRotator AimRotation)
{
	ProjectileMovement->Velocity = AimRotation.Vector() * InitialSpeed;
	ProjectileMovement->MaxSpeed = InitialSpeed;
	SetDamage(InDamage);
}

void AElementalBall::EnableOverlapEvents()
{
	if (SphereCollision)
	{
		SphereCollision->SetGenerateOverlapEvents(true);
	}
}

void AElementalBall::DisableOverlapEvents()
{
	if (SphereCollision)
	{
		SphereCollision->SetGenerateOverlapEvents(true);
	}
}

