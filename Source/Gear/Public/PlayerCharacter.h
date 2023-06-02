// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */

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
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> LookAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> SprintAction;

	// We bind Move function to the corresponding input action, so character can move 
	void Move(const FInputActionValue& Value);
	// We bind Look function to the corresponding input action, so character can look around 
	void Look(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Camera",meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCameraComponent> CameraComponent;

	void OnSprintUpdate(const FInputActionValue& Value);
	void OnEndSprint(const FInputActionValue& Value);
	
	void StartStaminaRecovery();
	void RecoverStamina(float DeltaTime);

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

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Camera",meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase> WalkCameraShake;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Camera",meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase> SprintCameraShake;

	EPlayerState PlayerState;

	FTimerHandle StaminaRecoveryTimerHandle;
	float StaminaRecoveryDelay;
	bool bStaminaRegenerates;
	
};
