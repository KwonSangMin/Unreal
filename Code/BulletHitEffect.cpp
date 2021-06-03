// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletHitEffect.h"

// Sets default values
ABulletHitEffect::ABulletHitEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	InitialLifeSpan = 0.7f;
	Scene=CreateDefaultSubobject<USceneComponent>(TEXT("loc"));
	RootComponent = Scene;
	Static = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Static->SetupAttachment(RootComponent);
	auto Mesh = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Burden/FX/P_Gideon_Burden_HitWorld.P_Gideon_Burden_HitWorld'"));
	if (Mesh.Succeeded())
	{
		Static->SetTemplate(Mesh.Object);
		Static->bAutoActivate = true;
	}
	//sound
	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("PlayerAudio"));
	Audio->bAutoActivate = true;
	
	Audio->SetupAttachment(RootComponent);
	auto Sound= ConstructorHelpers::FObjectFinder<USoundCue>(TEXT("SoundCue'/Game/Sounde/HIT_DS_17_Cue.HIT_DS_17_Cue'"));
	if (Sound.Succeeded())
		Cue = Sound.Object;
	
	
}

// Called when the game starts or when spawned
void ABulletHitEffect::BeginPlay()
{
	Super::BeginPlay();
	if (Audio)
	{
		Audio->SetSound(Cue);
		Audio->Play();
	}
}

// Called every frame
void ABulletHitEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

