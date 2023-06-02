// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"



APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// We create camera from c++, because we need quick access to the camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	CameraComponent->SetupAttachment(GetRootComponent());
	
	// Config camera component setting, so we can make first person camera look
	CameraComponent->bUsePawnControlRotation = true;
	CameraComponent->SetRelativeLocation(FVector(-10.f,0.f,60.f));
	
	MaxStamina = 100;
	StaminaDepletionRate = 10.f;
	StaminaRecoveryRate = 15.f;
	SprintSpeedModifier = 100.f;

	PlayerState = EPlayerState::Idle;

	StaminaRecoveryDelay = 5.0f;
	
	TargetShakeScale = 0.0f;
	CurrentShakeScale = 0.0f;
	ShakeScaleInterpSpeed = 1.f;
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	CurrentStamina = MaxStamina;
	SprintSpeedModifier += GetCharacterMovement()->MaxWalkSpeed;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add default Input Mapping Context
	if (const TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController()))
	{
		if (const TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext,0);
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(this))
	{
		return;
	}
	
#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp,Display,TEXT("Current Stamina: %f"),CurrentStamina);
#endif

	DecreaseStamina(DeltaTime);
	RecoverStamina(DeltaTime);	
	
	constexpr float VelocityThreshold = 0.01f;
	if (GetVelocity().SizeSquared() <= FMath::Square(VelocityThreshold))
	{
		PlayerState = EPlayerState::Idle;
	}
	UpdateCameraShake(DeltaTime);
	
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (const TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered,this,&APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Triggered,this,&APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(SprintAction,ETriggerEvent::Triggered,this,&APlayerCharacter::OnSprintUpdate);
		EnhancedInputComponent->BindAction(SprintAction,ETriggerEvent::Completed,this,&APlayerCharacter::OnEndSprint);
	}

}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D DirectionalValue = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// We take Y axis from DirectionalValue, so we can move forward/backward
		AddMovementInput(GetActorForwardVector(),DirectionalValue.Y);
		// We take X axis from DirectionalValue, so we can move left/right
		AddMovementInput(GetActorRightVector(),DirectionalValue.X);
		PlayerState = EPlayerState::Walking;
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// We take X axis from DirectionalValue, so we can look left/right
		AddControllerYawInput(LookAxisValue.X);
		// We take Y axis from DirectionalValue, so we can look up/down
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void APlayerCharacter::OnSprintUpdate(const FInputActionValue& Value)
{
	if (constexpr float VelocityThreshold = 0.01f;
		CurrentStamina <= 0 || GetVelocity().SizeSquared() <= FMath::Square(VelocityThreshold) || GetCharacterMovement()->IsFalling())
	{
		OnEndSprint(Value);
		return;
	}
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeedModifier;
	PlayerState = EPlayerState::Sprinting;
	bStaminaRegenerates = false;
	GetWorldTimerManager().ClearTimer(StaminaRecoveryTimerHandle);
}

void APlayerCharacter::OnEndSprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	PlayerState = EPlayerState::Walking;
	GetWorldTimerManager().SetTimer(StaminaRecoveryTimerHandle,this,&APlayerCharacter::StartStaminaRecovery,StaminaRecoveryDelay,false);
}


void APlayerCharacter::StartStaminaRecovery()
{
	bStaminaRegenerates = true;
}

void APlayerCharacter::RecoverStamina(float DeltaTime)
{
	if (!bStaminaRegenerates)
	{
		return;
	}
	if (CurrentStamina < MaxStamina)
	{
		CurrentStamina += StaminaRecoveryRate * DeltaTime;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, MaxStamina);
		return;
	}
	bStaminaRegenerates = false;
}

void APlayerCharacter::UpdateCameraShake(float DeltaTime)
{
	if (!GetCharacterMovement()->IsFalling())
	{
		switch (PlayerState)
		{
		case EPlayerState::Idle:
			TargetShakeScale = 0.f;
			break;
		case EPlayerState::Walking:
			TargetShakeScale = 1.f;
			break;
		case EPlayerState::Sprinting:
			TargetShakeScale = 2.f;
			break;
		}
		CurrentShakeScale = FMath::FInterpTo(CurrentShakeScale, TargetShakeScale, DeltaTime, ShakeScaleInterpSpeed);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(MovementCameraShake,CurrentShakeScale);
	}
}

void APlayerCharacter::DecreaseStamina(float DeltaTime)
{
	if (PlayerState == EPlayerState::Sprinting && CurrentStamina > 0)
	{
		CurrentStamina -= StaminaDepletionRate * DeltaTime;
	}
}
