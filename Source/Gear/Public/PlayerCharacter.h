// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */

// Enumeration defining player states
UENUM()
enum class EPlayerState : uint8
{
	Idle,
	Walking,
	Sprinting
};

UCLASS()
class GEAR_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	
	// Function called after components have been initialized
	virtual void PostInitializeComponents() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Pointers to various input actions
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> FireAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ReloadAction;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Camera",meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCameraComponent> CameraComponent;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	// Function to handle sprint input update
	void OnSprintUpdate(const FInputActionValue& Value);
	// Function to handle end of sprint input
	void OnEndSprint(const FInputActionValue& Value);
	// If current stamina less than max stamina and the player is not running, we recover stamina
	// (the number by which is added to the stamina is based on the StaminaRecoveryRate variable)
	void RecoverStamina(float DeltaTime);
	// If current stamina more than 0 and the player running, we decrease stamina
	// (the number by which is subtract to the stamina is based on the StaminaDepletionRate variable)
	void DecreaseStamina(float DeltaTime);
	// Update the camera shake effect based on the player state
	void UpdateCameraShake(float DeltaTime);
	
	void OnStartFire(const FInputActionValue& Value);
	void OnEndFire(const FInputActionValue& Value);

	void Reload(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Sprint",meta = (AllowPrivateAccess = true))
	float SprintSpeedModifier;
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Sprint",meta = (AllowPrivateAccess = true))
	float MaxStamina;
	float CurrentStamina;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Sprint",meta = (AllowPrivateAccess = true))
	float StaminaDepletionRate;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Sprint",meta = (AllowPrivateAccess = true))
	float StaminaRecoveryRate;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Camera|CameraShake",meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase> MovementCameraShake;

	EPlayerState PlayerState;
	
	float TargetShakeScale;
	float CurrentShakeScale;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Camera|CameraShake",meta = (AllowPrivateAccess = true))
	float ShakeScaleInterpSpeed;
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<class ABaseWeapon>> WeaponInventory;
	TObjectPtr<ABaseWeapon> CurrentWeapon;
};
