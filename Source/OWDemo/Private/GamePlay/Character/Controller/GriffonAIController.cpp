// If you want to use this project, mail to ggoggal@gmail.com

#include "GriffonAIController.h"

AGriffonAIController::AGriffonAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_GRIFFON
	(TEXT("BehaviorTree'/Game/Blueprints/Actors/Enemy/AI/BT_Griffon.BT_Griffon'"));
	if (BT_GRIFFON.Succeeded())
	{
		BTAsset = BT_GRIFFON.Object;
	}
}