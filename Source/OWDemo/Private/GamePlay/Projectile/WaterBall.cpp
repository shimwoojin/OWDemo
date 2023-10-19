// If you want to use this project, mail to ggoggal@gmail.com


#include "WaterBall.h"
#include "Particles/ParticleSystemComponent.h"

AWaterBall::AWaterBall()
{
	if (ElementalBallParticle)
	{
		ElementalBallParticle->SetupAttachment(GetRootComponent());

		static ConstructorHelpers::FObjectFinder<UParticleSystem> WATER_BALL
		(TEXT("ParticleSystem'/Game/Assets/FXVarietyPack/Particles/P_ky_waterBall.P_ky_waterBall'"));
		if (WATER_BALL.Succeeded())
		{
			ElementalBallParticle->SetTemplate(WATER_BALL.Object);
			ElementalBallParticle->SetRelativeScale3D(FVector(.3f, .3f, .3f));
		}

		static ConstructorHelpers::FObjectFinder<UParticleSystem> WATER_BALL_HITEFFECT
		(TEXT("ParticleSystem'/Game/Assets/FXVarietyPack/Particles/P_ky_waterBallHit.P_ky_waterBallHit'"));
		if (WATER_BALL_HITEFFECT.Succeeded())
		{
			ElementalBallHitEffect = WATER_BALL_HITEFFECT.Object;
		}
	}
}