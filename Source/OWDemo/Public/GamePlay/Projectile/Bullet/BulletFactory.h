// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "OWEnums.h"
#include "UObject/NoExportTypes.h"
#include "BulletFactory.generated.h"

UCLASS()
class OWDEMO_API UBulletFactory : public UObject
{
	GENERATED_BODY()
	
public:
	
	UBulletFactory* SetSpawnDirectionAndLocation(FRotator InSpawnDirection, FVector InSpawnLocation);
	UBulletFactory* SetStaticmesh(class UStaticMesh* InMesh);
	UBulletFactory* SetWeaponType(ESoldierWeaponType InWeaponType);
	UBulletFactory* SetSpeed(float InSpeed);
	UBulletFactory* SetDamage(float InDamage);
	UBulletFactory* SetOwner(AActor* InOwner);
	UBulletFactory* SetInstigator(APawn* InInstigator);

	class ABullet* BuildBullet();

private:
	FRotator SpawnDirection;
	FVector SpawnLocation;

	AActor* Owner;
	APawn* Instigator;

	class UStaticMesh* Mesh;

	ESoldierWeaponType WeaponType;

	float Speed;
	float Damage;
};
