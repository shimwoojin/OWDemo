// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "BaseCharacter.h"
#include "BaseEnemy.generated.h"

UCLASS()
class OWDEMO_API ABaseEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float AddCurrentHP(float AddHP) final;

public:
	class UWeaponComponent* GetWeaponManager() const { return WeaponManager; }

private:
	void ShowHPGauge();
	UFUNCTION()
		void HideHPGauge();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = true))
		class UWeaponComponent* WeaponManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HP, meta = (AllowPrivateAccess = true))
		class UWidgetComponent* HPGauge;

	float ShowHPGaugeTime;
};
