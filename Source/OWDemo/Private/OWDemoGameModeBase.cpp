// Copyright Epic Games, Inc. All Rights Reserved.


#include "OWDemoGameModeBase.h"
#include "BasePlayerController.h"
#include "PlayerShinbi.h"
#include "Kismet/GameplayStatics.h"

AOWDemoGameModeBase::AOWDemoGameModeBase()
{
	PlayerControllerClass = ABasePlayerController::StaticClass();
	DefaultPawnClass = APlayerShinbi::StaticClass();
}

void AOWDemoGameModeBase::StartPlay()
{
	Super::StartPlay();


	//auto* GamePlaySound = LoadObject<USoundBase>(this, TEXT("SoundCue'/Game/Assets/Sounds/FantasyOrchestral/cues/ElvenRealm.ElvenRealm'"));
	//if (GamePlaySound)
	//{
	//	float Duration = GamePlaySound->GetDuration();
	//	GetWorldTimerManager().SetTimer(BackgroundSoundTimer, FTimerDelegate::CreateLambda([this, GamePlaySound]() {
	//		UGameplayStatics::PlaySound2D(this, GamePlaySound, BackgroundSoundMultiplier);
	//		}), Duration, true, 0.f);
	//}
	//else
	//{
	//	LOG(Warning, TEXT("Not Loaded GamePlaySound"));
	//}
}

void AOWDemoGameModeBase::PauseBackgroundMusic()
{
	GetWorldTimerManager().PauseTimer(BackgroundSoundTimer);
}

void AOWDemoGameModeBase::PlayBackgroundMusic()
{
	GetWorldTimerManager().UnPauseTimer(BackgroundSoundTimer);
}
