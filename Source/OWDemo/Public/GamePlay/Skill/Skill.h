// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "GameFramework/Actor.h"
#include "SkillActivateManagement.h"
#include "Skill.generated.h"

UCLASS(Abstract)
class OWDEMO_API ASkill : public AActor, public ISkillActivateManagement
{
	GENERATED_BODY()
	
public:	
	ASkill();

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnSkillStartNotify() override {}
	virtual void OnSkillEndNotify() override {}

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		virtual bool CanUseSkill();

	UFUNCTION()
		virtual bool Activate();

	UFUNCTION()
		virtual void OnKeyReleased() {}

	UFUNCTION()
		void SkillCanUse();

	UFUNCTION()
		void SkillCanNotUse();

	virtual float GetSkillUsagePercent() { return 0.f; }

public:
	UPROPERTY(EditDefaultsOnly, Category = Image)
		class UTexture2D* SkillTexture;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Skill, meta = (AllowPrivateAccess = true))
		bool bCanUseSkill;

protected:
	UPROPERTY(EditDefaultsOnly)
		class UMontagePlayDataAsset* SkillMontage;
};
