// If you want to use this project, mail to ggoggal@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterBaseComponent.generated.h"

class ABaseCharacter;

UCLASS( ClassGroup=(CharacterBase), meta=(BlueprintSpawnableComponent) )
class OWDEMO_API UCharacterBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterBaseComponent();

protected:
	virtual void BeginPlay() override;
	
};
