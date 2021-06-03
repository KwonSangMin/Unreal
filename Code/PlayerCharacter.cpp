// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	auto SKeleMesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/ParagonGideon/Characters/Heroes/Gideon/Meshes/Gideon_soc.Gideon_soc'"));
	SPA = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SPA->SetupAttachment(RootComponent);
	Cam->SetupAttachment(SPA, USpringArmComponent::SocketName);
	SPA->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, BaseEyeHeight), FRotator(0.0f, 90.0f, 0.0f));
	SPA->bUsePawnControlRotation = true;
	SPA->TargetArmLength = 500.f;
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));//Capsule Height ==88

	stun = false;
	speed = false;
	NickName = "SANGMIN";
	RMBullet = nullptr;
	HP = 2000;
	MAXHP = 2000;
	Maxcombo = 2;
	RMB = false;
	FIREBlackHole = false;
	resetcombo();

	auto Montage = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("AnimMontage'/Game/Blueprints/NewAnimMontage.NewAnimMontage'"));
	if (Montage.Succeeded())
		AttackMontage = Montage.Object;
	auto DMontage = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("AnimMontage'/Game/Blueprints/Death.Death'"));
	if (DMontage.Succeeded())
		DeadMontage = DMontage.Object;
	
	auto Anim = ConstructorHelpers::FClassFinder<UAnimInstance>(TEXT("AnimBlueprint'/Game/Blueprints/PA.PA_C'"));
	if (Anim.Class != nullptr)
	{
		GetMesh()->SetAnimInstanceClass(Anim.Class);
	}
	if (SKeleMesh.Object != nullptr)
	{
		GetMesh()->SetSkeletalMesh(SKeleMesh.Object);
	}
}
// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	if(this)
	{
		Super::Tick(DeltaTime);
		IsInAir = GetMovementComponent()->IsFalling();
		if (GetVelocity().IsZero()&&speed==true)
			Run();	
		if (speed == false && GetCharacterMovement()->MaxWalkSpeed > 600)
		{
			GetCharacterMovement()->MaxWalkSpeed -= 5;
		}	
		if (RMBullet)
		{
			FVector EyeLocation;
			FRotator EyeRotation;
			GetActorEyesViewPoint(EyeLocation, EyeRotation);
			FVector LaunchDirection = EyeRotation.Vector();
			LaunchDirection.Normalize();
			EyeLocation += LaunchDirection * (80* RMBullet->Bulletsize);
			RMBullet->SetActorLocationAndRotation(EyeLocation, EyeRotation);
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	InputComponent->BindAxis("Turn", this, &APlayerCharacter::Z_Yaw);
	InputComponent->BindAxis("LookUp", this, &APlayerCharacter::Y_Pitch);
	InputComponent->BindAction("Run",IE_Pressed, this, &APlayerCharacter::Run);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::LMB);
	InputComponent->BindAction("BlackHole", IE_Pressed, this, &APlayerCharacter::BlackHole);
	InputComponent->BindAction("BlackHole", IE_Released, this, &APlayerCharacter::CanFire);
}
void APlayerCharacter::LMB()
{
	if (CurrentCombo >= 3||stun)
	{

	}
	else{
	if (IsAttack == false)
	{
		Attack();
	}
	else if (IsAttack == true)
	{
		CanNextCombo = true;
	}
	}
}
void APlayerCharacter::Attack()
{
	
		if (AttackMontage != nullptr&&IsAttack==false)
		{
			IsAttack = true;
			CanNextCombo = false;
			auto Instance = GetMesh()->GetAnimInstance();
			Instance->Montage_Play(AttackMontage);
			Instance->Montage_JumpToSection(FName(comboList[CurrentCombo]), AttackMontage);
			
			CurrentCombo += 1;
		}
		else if (IsAttack = true && CanNextCombo == true)
		{
			auto Instance = GetMesh()->GetAnimInstance();
			Instance->Montage_Play(AttackMontage);
			Instance->Montage_JumpToSection(FName(comboList[CurrentCombo]), AttackMontage);
			CurrentCombo += 1;
			CanNextCombo = false;
		}
}
void APlayerCharacter::MoveForward(float Axisvalue)
{
	if (Controller != nullptr && Axisvalue != 0.f)
	{
		FVector Direction = FRotationMatrix(GetActorRotation()).GetUnitAxis(EAxis::X);
		Direction.Z = 0;
		Direction.Normalize();
		AddMovementInput(Direction, Axisvalue);
	}
}

void APlayerCharacter::MoveRight(float Axisvalue)
{
	if (Controller != nullptr && Axisvalue != 0.f )
	{
		FVector Direction = FRotationMatrix(GetActorRotation()).GetUnitAxis(EAxis::Y);
		Direction.Z = 0;
		Direction.Normalize();
		AddMovementInput(Direction, Axisvalue);
	}
}
void APlayerCharacter::Z_Yaw(float Axisvalue)
{
	if (Axisvalue != 0.f)
	{
		AddControllerYawInput(Axisvalue);
	}
}
void APlayerCharacter::Y_Pitch(float Axisvalue)
{
	if (Axisvalue != 0.f)
	{
		AddControllerPitchInput(Axisvalue);
	}
}

void APlayerCharacter::Run()
{
	FTimerHandle Timer;
	speed = !speed;
	if (speed)
		GetCharacterMovement()->MaxWalkSpeed = 990;
	
}
void APlayerCharacter::AttackEnd()
{
	IsAttack = false;
	resetcombo();
}
void APlayerCharacter::Combo()
{
	if (CanNextCombo)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combo with Can Next Combo"));
		Attack();
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Combo withOut Can Next Combo"));
}
void APlayerCharacter::Fire()
{
	FVector EyeLocation;
	FRotator EyeRotation;
	GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	UWorld* World = GetWorld();
	if (CurrentCombo != 2)
	{
		EyeLocation = GetMesh()->GetSocketLocation(TEXT("weapon_soc"));
		
	}
	else
		EyeLocation = GetMesh()->GetSocketLocation(TEXT("hand_l"));
	auto BULLET = World->SpawnActor<ABullet>(ABullet::StaticClass(), EyeLocation, EyeRotation, SpawnParams);
	if (BULLET)
	{
		FVector LaunchDirection = EyeRotation.Vector();
		BULLET->Player = this;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("BULLET CREATE FAIL"));
	}
}
void APlayerCharacter::resetcombo()
{
	UE_LOG(LogTemp, Warning, TEXT("ResetCombo"));
	IsAttack = false;
	CanNextCombo = false;
	CurrentCombo = 0;
	FIREBlackHole = false;
}
void APlayerCharacter::BlackHole()
{
	if (IsAttack == false && !RMBullet && !stun)
	{
		auto Instance = GetMesh()->GetAnimInstance();
		IsAttack = true;
		Instance->Montage_Play(AttackMontage);
		Instance->Montage_JumpToSection(FName("RMB"), AttackMontage);
		RMB = true;
	}
	else
	{
		
	}
}
void APlayerCharacter::BlackHoleCreate()
{
	
	FVector EyeLocation;
	FRotator EyeRotation;
	GetActorEyesViewPoint(EyeLocation, EyeRotation);
	channum = 0;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	
	FVector LaunchDirection = EyeRotation.Vector();
	LaunchDirection.Normalize();
	EyeLocation += LaunchDirection * 100;
	UWorld* World = GetWorld();
		
	RMBullet = World->SpawnActor<ARMBBullet>(ARMBBullet::StaticClass(), EyeLocation, EyeRotation, SpawnParams);

}
void APlayerCharacter::CanFire()
{
	if(RMBullet||RMB)
		FIREBlackHole = true;
}
void APlayerCharacter::RMBFire()
{
		UE_LOG(LogTemp, Warning, TEXT("RMBFIRE"));
		
		if (RMBullet)
		{
			auto Instance = GetMesh()->GetAnimInstance();
			channum = 0;
			Instance->Montage_Play(AttackMontage);
			Instance->Montage_JumpToSection(FName("RMBfire"), AttackMontage);
			FVector EyeLocation;
			FRotator EyeRotation;
		
			RMBullet->Player = this;
			RMBullet->FireInDirection();
			RMBullet = nullptr;
			RMB = false;
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("RMBullet CREATE FAIL"));
		}
}
void APlayerCharacter::Jump()
{
	bPressedJump = true;
	JumpKeyHoldTime = 0.0f;
}
void APlayerCharacter::Chan()
{
	auto Instance = GetMesh()->GetAnimInstance();
	Instance->Montage_Play(AttackMontage);
	Instance->Montage_JumpToSection(FName("LOOP"), AttackMontage);
	
	
	if (channum >= 10)
		RMBFire();
	if (RMBullet && FIREBlackHole == false)
	{
		RMBullet->UpdateBulletSize();
		channum++;
	}
	else if (RMBullet && FIREBlackHole == true)
		RMBFire();
	else {
		UE_LOG(LogTemp, Log, TEXT("RMBullet CREATE FAIL"));
	}
}

void APlayerCharacter::Death()
{
	
	auto Instance = GetMesh()->GetAnimInstance();
	Instance->Montage_Play(DeadMontage);
	Instance->Montage_JumpToSection(FName("Default"), DeadMontage);
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (RMBullet)
		RMBullet->Destroy();
	
	GetController()->GetPawn()->UnPossessed();
}
float APlayerCharacter::TakeDamage(float EDamage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageCauser)
	{
		HP -= EDamage;
		float DamageYaw = 0;
		FVector UnitDirection = DamageCauser->GetActorLocation() - GetActorLocation();
		UnitDirection.Normalize();
		FVector DirecXY = UnitDirection * FVector(1.f, 1.f, 0.f);
		FVector Origin = GetActorForwardVector();
		float InnerProduct = FVector::DotProduct(DirecXY, Origin);
		DamageYaw = UKismetMathLibrary::DegAcos(InnerProduct);
		FVector OutterProduct = FVector::CrossProduct(Origin, DirecXY);
		float DegSign = UKismetMathLibrary::SignOfFloat(OutterProduct.Z);
		HitYaw= DamageYaw * DegSign;
		if (EDamage >= MAXHP * 0.1)//Stun
		{
			if (RMBullet)
				RMBFire();
			resetcombo();
			stun = true;
		}
		if (HP <= 0)
			Death();
		UE_LOG(LogTemp, Log, TEXT("Current HP %d"),HP);
		return HitYaw;
	}
	else
		return 0;
}