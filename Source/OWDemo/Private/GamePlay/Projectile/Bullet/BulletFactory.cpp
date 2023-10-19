// If you want to use this project, mail to ggoggal@gmail.com


#include "BulletFactory.h"
#include "Bullet.h"

UBulletFactory* UBulletFactory::SetSpawnDirectionAndLocation(FRotator InSpawnDirection, FVector InSpawnLocation)
{
	SpawnDirection = InSpawnDirection;
	SpawnLocation = InSpawnLocation;
	return this;
}

UBulletFactory* UBulletFactory::SetSpeed(float InSpeed)
{
	Speed = InSpeed;
	return this;
}

UBulletFactory* UBulletFactory::SetDamage(float InDamage)
{
	Damage = InDamage;
	return this;
}

UBulletFactory* UBulletFactory::SetOwner(AActor* InOwner)
{
	Owner = InOwner;
	return this;
}

UBulletFactory* UBulletFactory::SetInstigator(APawn* InInstigator)
{
	Instigator = InInstigator;
	return this;
}

UBulletFactory* UBulletFactory::SetStaticmesh(class UStaticMesh* InMesh)
{
	Mesh = InMesh;
	return this;
}

UBulletFactory* UBulletFactory::SetWeaponType(ESoldierWeaponType InWeaponType)
{
	WeaponType = InWeaponType;
	return this;
}

ABullet* UBulletFactory::BuildBullet()
{
	FTransform SpawnTransform(SpawnDirection, SpawnLocation);

	UWorld* World = GetWorld();

	if (World)
	{
		auto* Bullet = GetWorld()->SpawnActorDeferred<ABullet>(ABullet::StaticClass(), SpawnTransform, Owner, Instigator);

		if (Bullet)
		{
			Bullet->Initialize(Speed, Damage, Mesh, WeaponType);
			Bullet->FinishSpawning(SpawnTransform);

			return Bullet;
		}
	}
	else
	{
		LOG(Warning, TEXT("Bullet Did Not Spawned, Reason : No World!!"));
	}

	return nullptr;
}
