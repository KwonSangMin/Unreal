// Fill out your copyright notice in the Description page of Project Settings.


#include "RMBBullet.h"

// Sets default values
ARMBBullet::ARMBBullet() 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Damage = 200;
	Bulletsize = 1;
	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere")); // 충돌 판정 컴포넌트 ( 구체형 )
	sphere->InitSphereRadius(50.f);
	
	RootComponent = sphere;
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	ProjectileMovementComponent->SetUpdatedComponent(sphere);
	ProjectileMovementComponent->InitialSpeed = 0;
	ProjectileMovementComponent->MaxSpeed = 5000;
	ProjectileMovementComponent->ProjectileGravityScale = 0;

	//투사체
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));;
	Mesh->SetupAttachment(RootComponent);
	auto SMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/ParagonGideon/FX/Meshes/Heroes/Gideon/SM_Black_Hole_Main.SM_Black_Hole_Main'"));
	if (SMesh.Succeeded())
	{
		Mesh->SetStaticMesh(SMesh.Object);
		Mesh->SetRelativeLocation(FVector(0, 0, 0));
	}
	//비행 이펙트
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileParticle"));
	Particle->SetupAttachment(RootComponent);
	
	auto Effect = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Ultimate/FX/P_Gideon_BlackHole_Beam.P_Gideon_BlackHole_Beam'"));
	if (Effect.Succeeded())
	{
		Particle->SetTemplate(Effect.Object);
		Particle->bAutoActivate = true;
	}

	OuterParticle= CreateDefaultSubobject<UParticleSystemComponent>(TEXT("OuterParticle"));
	OuterParticle->SetupAttachment(RootComponent);
	auto OEffect = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Skins/Undertow/P_Gideon_Ultimate_Undertow.P_Gideon_Ultimate_Undertow'"));
	if (OEffect.Succeeded())
	{
		OuterParticle->SetTemplate(OEffect.Object);
		OuterParticle->bAutoActivate = true;
	}
}

// Called when the game starts or when spawned
void ARMBBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARMBBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ARMBBullet::FireInDirection()
{
	ProjectileMovementComponent->Velocity = 5000*GetActorRotation().Vector();
	sphere->BodyInstance.SetCollisionProfileName(TEXT("bullet2"));
	sphere->OnComponentBeginOverlap.AddDynamic(this, &ARMBBullet::OnOverlapBegin);
	sphere->OnComponentHit.AddDynamic(this, &ARMBBullet::OnHit);
	SetLifeSpan(5.f);
}

void ARMBBullet::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Monster"))
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, Damage,OtherActor->GetActorLocation(),SweepResult ,Player->GetController(), this, NULL);
	}
	SetLifeSpan(0.5f);
	// 액터에 닿을 시 불렛 제거
}
void ARMBBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	
	Destroy();
	// 액터에 닿을 시 불렛 제거
}
void ARMBBullet::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
	if (HitLocation == FVector(0.f, 0.f, 0.f))
		HitLocation = GetActorLocation();
	FActorSpawnParameters SpawnParams;
	auto BulletHitEffect = GetWorld()->SpawnActor<ARMBEffect>(ARMBEffect::StaticClass(), HitLocation, FRotator(0, 0, 0), SpawnParams);
	if(BulletHitEffect)
		BulletHitEffect->RUN(Bulletsize);
	Super::EndPlay(EndPlayReason);

}
void ARMBBullet::UpdateBulletSize()
{
	Bulletsize += 0.1;
	Mesh->SetWorldScale3D(FVector(1.f, 1.f, 1.f) * Bulletsize);
	Particle->SetWorldScale3D(FVector(1.f, 1.f, 1.f) * Bulletsize);
	OuterParticle->SetWorldScale3D(FVector(2.f, 2.f, 2.f) * Bulletsize);
	sphere->SetSphereRadius(50.f * Bulletsize);
	Damage *= Bulletsize;
}
