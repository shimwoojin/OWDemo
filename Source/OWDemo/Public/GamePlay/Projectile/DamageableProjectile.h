// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "BaseProjectile.h"
#include "DamageableProjectile.generated.h"

UCLASS(Abstract)
class OWDEMO_API ADamageableProjectile : public ABaseProjectile
{
	GENERATED_BODY()
	
public:
	ADamageableProjectile();

public:
	float GetDamage() { return Damage; }
	void SetDamage(float InDamage) { Damage = InDamage; }

private:
	float Damage;
};
