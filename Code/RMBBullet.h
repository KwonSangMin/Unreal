// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletClass.h"
#include "RMBEffect.h"
#include "RMBBullet.generated.h"

UCLASS()
class CHAPTER5_API ARMBBullet : public ABulletClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARMBBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UPROPERTY(VisibleAnywhere, Category = Camera)
		UStaticMeshComponent* Mesh;
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void FireInDirection();
	void UpdateBulletSize();
	UParticleSystemComponent* OuterParticle;
};
