// If you want to use this project, mail to ggoggal@gmail.com


#include "BulletTracerDataAsset.h"
#include "Particles/ParticleSystemComponent.h"

UBulletTracerDataAsset::UBulletTracerDataAsset()
{

}

UParticleSystem* UBulletTracerDataAsset::GetTracerParticle(ESoldierWeaponType WeaponType)
{
	if (TracerParticleMap.Find(WeaponType))
	{
		return TracerParticleMap[WeaponType];
	}

	return nullptr;
}