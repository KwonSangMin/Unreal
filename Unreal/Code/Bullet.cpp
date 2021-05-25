// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	InitialLifeSpan = 3.0f;
	Damage = 100;
	Bulletsize = 1;
	HitLocation = FVector(0.f, 0.f, 0.f);
	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere")); // 충돌 판정 컴포넌트 ( 구체형 )
	sphere->InitSphereRadius(30.f);
	sphere->BodyInstance.SetCollisionProfileName(TEXT("bullet"));
	sphere->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
	RootComponent = sphere;
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	ProjectileMovementComponent->SetUpdatedComponent(sphere);
	ProjectileMovementComponent->InitialSpeed = 5000;
	ProjectileMovementComponent->MaxSpeed = 5000;
	ProjectileMovementComponent->ProjectileGravityScale = 0;

	//투사체
	Static = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));;
	Static->SetupAttachment(RootComponent);
	auto Mesh=ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Burden/FX/P_Gideon_Burden_DoT.P_Gideon_Burden_DoT'"));
	if (Mesh.Succeeded())
	{
		Static->SetTemplate(Mesh.Object);
		Static->bAutoActivate = true;
	}
	//비행 이펙트
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileParticle"));
	Particle->SetupAttachment(RootComponent);
	auto Effect = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Burden/FX/P_Gideon_Burden_Projectile.P_Gideon_Burden_Projectile'"));
	if (Effect.Succeeded())
	{
		Particle->SetTemplate(Effect.Object);
		Particle->bAutoActivate = true;
	}
	
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherActor->ActorHasTag("Monster"))
	{
		HitLocation = Hit.ImpactPoint;
		UGameplayStatics::ApplyDamage(OtherActor, Damage, Player->GetController(), this, NULL);
	}
	else if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity*2, Hit.ImpactPoint);
	}
	Destroy();
	 // 액터에 닿을 시 불렛 제거
}
// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABullet::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (HitLocation == FVector(0.f, 0.f, 0.f))
		HitLocation = GetActorLocation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Player;
	auto BulletHitEffect = GetWorld()->SpawnActor<ABulletHitEffect>(ABulletHitEffect::StaticClass(), HitLocation, FRotator(0,0,0), SpawnParams);
	Super::EndPlay(EndPlayReason);

}

