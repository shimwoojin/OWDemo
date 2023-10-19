// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "OWDemo.h"
#include "GameFramework/GameModeBase.h"
#include "OWDemoGameModeBase.generated.h"

UCLASS()
class OWDEMO_API AOWDemoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AOWDemoGameModeBase();

	virtual void StartPlay() override;

public:
	void PauseBackgroundMusic();
	void PlayBackgroundMusic();

private:
	FTimerHandle BackgroundSoundTimer;

	UPROPERTY(EditAnywhere)
		float BackgroundSoundMultiplier = .6f;
};
