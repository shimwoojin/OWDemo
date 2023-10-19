// If you want to use this project, mail to ggoggal@gmail.com

#include "ThunderBall.h"
#include "Particles/ParticleSystemComponent.h"

AThunderBall::AThunderBall()
{
	if (ElementalBallParticle)
	{
		ElementalBallParticle->SetupAttachment(GetRootComponent());

		static ConstructorHelpers::FObjectFinder<UParticleSystem> THUNDER_BALL
		(TEXT("ParticleSystem'/Game/Assets/FXVarietyPack/Particles/P_ky_thunderBall.P_ky_thunderBall'"));
		if (THUNDER_BALL.Succeeded())
		{
			ElementalBallParticle->SetTemplate(THUNDER_BALL.Object);
			ElementalBallParticle->SetRelativeScale3D(FVector(.3f, .3f, .3f));
		}

		static ConstructorHelpers::FObjectFinder<UParticleSystem> THUNDER_BALL_HITEFFECT
		(TEXT("ParticleSystem'/Game/Assets/FXVarietyPack/Particles/P_ky_ThunderBallHit.P_ky_ThunderBallHit'"));
		if (THUNDER_BALL_HITEFFECT.Succeeded())
		{
			ElementalBallHitEffect = THUNDER_BALL_HITEFFECT.Object;
		}
	}
}
