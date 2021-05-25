// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "BulletHitEffect.generated.h"

UCLASS()
class CHAPTER5_API ABulletHitEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletHitEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = Status)
		UParticleSystemComponent* Static;
	UPROPERTY(EditAnywhere, Category = Status)
		USceneComponent* Scene;
	UPROPERTY(EditAnywhere, Category = Status)
		USoundCue* Cue;
	UPROPERTY(EditAnywhere, Category = Status)
		UAudioComponent* Audio;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
