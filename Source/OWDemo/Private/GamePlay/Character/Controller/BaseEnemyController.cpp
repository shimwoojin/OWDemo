// If you want to use this project, mail to ggoggal@gmail.com

#include "BaseEnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BasePlayer.h"

const FName ABaseEnemyController::TargetKey(TEXT("Target"));
const FName ABaseEnemyController::bLookTargetKey(TEXT("bLookTarget"));

void ABaseEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BTAsset)
	{
		RunBehaviorTree(BTAsset);
	}
}

void ABaseEnemyController::SetTarget(class ABaseCharacter* InTarget)
{
	if (GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsObject(TargetKey, InTarget);
		Target = InTarget;
	}
	else
	{
		LOG(Warning, TEXT("ABaseEnemyController::SetTarget() Failed"));
	}
}

void ABaseEnemyController::LookTarget(bool InbLookTarget)
{
	if (GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsBool(bLookTargetKey, InbLookTarget);
		bLookTarget = InbLookTarget;
	}
	else
	{
		LOG(Warning, TEXT("ABaseEnemyController::LookTarget() Failed"));
	}
}