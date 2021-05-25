// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Bullet.h"
#include "RMBBullet.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class CHAPTER5_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	UAnimMontage* AttackMontage;
	ARMBBullet* RMBullet;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SPA;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
		bool RMB;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = Camera)
		UCameraComponent* Cam;
	void MoveForward(float Axisvalue);
	void MoveRight(float Axisvalue);
	void Attack();
	void LMB();
	void Y_Pitch(float Axisvalue);
	void Z_Yaw(float Axisvalue);
	void Run();
	UPROPERTY(EditAnywhere, Category = Status)
		bool speed;
	UPROPERTY(EditAnywhere, Category = Status)
		int HP;
	UPROPERTY(EditAnywhere, Category = Status)
		FString NickName;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Status)
		bool IsInAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
		bool IsAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
		FVector Dir;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
		int32 Maxcombo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
		int32 CurrentCombo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
		bool CanNextCombo;
	void resetcombo();
	void BlackHole();
	void Jump();
	const char* comboList[6] = { "Attack1", "Attack2","Attack3","RMB","RMBfire","LOOP" };
public:
	UFUNCTION(BlueprintCallable)
		void AttackEnd();
	UFUNCTION(BlueprintCallable)
		void Combo();
	UFUNCTION(BlueprintCallable)
		void Fire();
	UFUNCTION(BlueprintCallable)
		void RMBFire();
	UFUNCTION(BlueprintCallable)
		void Chan();
	
};
