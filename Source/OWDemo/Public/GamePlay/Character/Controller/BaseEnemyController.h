// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "OWDemo.h"
#include "AIController.h"
#include "BaseEnemyController.generated.h"

UCLASS()
class OWDEMO_API ABaseEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void OnPossess(APawn* InPawn) override;

public:
	static const FName TargetKey;
	static const FName bLookTargetKey;

public:
	class ABaseCharacter* GetTarget() { return Target; }
	void SetTarget(class ABaseCharacter* InTarget);
	void LookTarget(bool InbLookTarget);

private:
	class ABaseCharacter* Target;
	bool bLookTarget;

protected:
	UPROPERTY(EditDefaultsOnly)
		class UBehaviorTree* BTAsset;


};
