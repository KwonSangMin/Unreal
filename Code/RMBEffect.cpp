// Fill out your copyright notice in the Description page of Project Settings.


#include "RMBEffect.h"

// Sets default values
ARMBEffect::ARMBEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	InitialLifeSpan = 3.f;
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("loc"));
	RootComponent = Scene;
	Static = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Static->SetupAttachment(RootComponent);
	auto Mesh = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Meteor/FX/P_Gideon_Meteor_Portal_Fast.P_Gideon_Meteor_Portal_Fast'"));
	if (Mesh.Succeeded())
	{
		Static->SetTemplate(Mesh.Object);
		Static->bAutoActivate = false;
	}
	else
	{
		Destroy();
	}
	//sound
	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("PlayerAudio"));
	Audio->bAutoActivate = false;
	
	Audio->SetupAttachment(RootComponent);
	auto Sound = ConstructorHelpers::FObjectFinder<USoundCue>(TEXT("SoundCue'/Game/Sounde/STINGERS_11_Cue.STINGERS_11_Cue'"));
	if (Sound.Succeeded())
		Cue = Sound.Object;
	else
	{
		Destroy();
	}
}

// Called when the game starts or when spawned
void ARMBEffect::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARMBEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ARMBEffect::RUN(float size)
{
	if (Static)
	{
		Static->Activate();
		Static->SetWorldScale3D(FVector(1.f, 1.f, 1.f) * size);
	}
	if (Audio)
	{
		Audio->Activate();
		Audio->SetSound(Cue);
		Audio->Play();
	}
}

