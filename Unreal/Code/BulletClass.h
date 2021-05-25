// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "BulletHitEffect.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "BulletClass.generated.h"

UCLASS(abstract)
class CHAPTER5_API ABulletClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletClass();
	unsigned int Damage;
	float Bulletsize;

	APawn* Player; // 소유 객체

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	USphereComponent* sphere;
	UParticleSystemComponent* Static;
	UParticleSystemComponent* Particle;
	UProjectileMovementComponent* ProjectileMovementComponent;
	FVector HitLocation; // 타격 위치

};
