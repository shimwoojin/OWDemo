// If you want to use this project, mail to ggoggal@gmail.com

#include "TreasureBox.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Sound/SoundCue.h"

ATreasureBox::ATreasureBox()
{
	CreateDefaultSubobjectAuto(Box);
	CreateDefaultSubobjectAuto(Mesh);

	if (Box)
	{
		SetRootComponent(Box);
		Box->SetCollisionProfileName(OWCollision::PresetName_InteractableItem);
		Box->SetBoxExtent(FVector(32.f, 50.f, 50.f));
	}

	if (Mesh)
	{
		Mesh->SetupAttachment(GetRootComponent());
		Mesh->SetCollisionProfileName(OWCollision::PresetName_NoCollision);
		Mesh->SetRelativeLocation(FVector(0.f, 0.f, -50.f));
	}

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> PARTICLE
	(TEXT("NiagaraSystem'/Game/Assets/sA_PickupSet_1/Fx/NiagaraSystems/NS_Coin_Shower.NS_Coin_Shower'"));
	if (PARTICLE.Succeeded())
	{
		Particle = PARTICLE.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundCue> SOUND
	(TEXT("SoundCue'/Game/Assets/InterfaceAndItemSounds/Cues/Coins_15_wav_Cue.Coins_15_wav_Cue'"));
	if (PARTICLE.Succeeded())
	{
		Sound = SOUND.Object;
	}

	ParticleStartHeight = 50.f;
}

void ATreasureBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (SpawnItemClass != nullptr)
	{
		GetWorld()->SpawnActor<AItem>(SpawnItemClass, GetActorLocation(), GetActorRotation());
		FVector Location = GetActorLocation() + FVector(0.f, 0.f, ParticleStartHeight);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Particle, Location, GetActorRotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sound, Location, GetActorRotation());
	}

	Destroy();
}
