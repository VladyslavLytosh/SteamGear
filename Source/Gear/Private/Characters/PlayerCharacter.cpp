// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"

#include "Weapons/BaseWeapon.h"
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
	
	// Configure camera component settings for first-person perspective
	CameraComponent->bUsePawnControlRotation = true;
	CameraComponent->SetRelativeLocation(FVector(-10.f,0.f,60.f));

	// Attach PlayerMesh to the Camera
	GetMesh()->SetupAttachment(CameraComponent);
	
	// Initialize player stamina-related and sprint variables
	MaxStamina = 100;
	StaminaDepletionRate = 10.f;
	StaminaRecoveryRate = 15.f;
	SprintSpeedModifier = 100.f;
	
	// Set the initial player state to idle
	PlayerState = EPlayerState::Idle;
	
	// Initialize camera shake variables
	TargetShakeScale = 0.0f;
	CurrentShakeScale = 0.0f;
	ShakeScaleInterpSpeed = 1.f;

}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	
	// Initialize current stamina to the maximum value
	CurrentStamina = MaxStamina;
	
	// Set the sprint speed modifier to the maximum walking speed plus the sprint speed modifier
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
	CurrentWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponInventory[0], GetActorLocation(), GetActorRotation());
	CurrentWeapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,"Weapon_socket");
	CurrentWeapon->SetOwner(this);

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Skip further processing if the character is not valid
	if (!IsValid(this))
	{
		return;
	}

	// Decrease the player's stamina and recover it based on the player state and current stamina number
	DecreaseStamina(DeltaTime);
	RecoverStamina(DeltaTime);	
	
	constexpr float VelocityThreshold = 0.01f;
	if (GetVelocity().SizeSquared() <= FMath::Square(VelocityThreshold))
	{
		PlayerState = EPlayerState::Idle;
	}
	//UpdateCameraShake(DeltaTime);
	
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Bind various input actions to corresponding functions
	if (const TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered,this,&APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Triggered,this,&APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(SprintAction,ETriggerEvent::Triggered,this,&APlayerCharacter::OnSprintUpdate);
		EnhancedInputComponent->BindAction(SprintAction,ETriggerEvent::Completed,this,&APlayerCharacter::OnEndSprint);
		EnhancedInputComponent->BindAction(FireAction,ETriggerEvent::Started,this,&APlayerCharacter::OnStartFire);
		EnhancedInputComponent->BindAction(FireAction,ETriggerEvent::Completed,this,&APlayerCharacter::OnEndFire);
		EnhancedInputComponent->BindAction(ReloadAction,ETriggerEvent::Started,this,&APlayerCharacter::Reload);
	}

}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D DirectionalValue = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Move the character forward/backward based on the Y axis of the DirectionalValue
		AddMovementInput(GetActorForwardVector(), DirectionalValue.Y);
		// Move the character left/right based on the X axis of the DirectionalValue
		AddMovementInput(GetActorRightVector(), DirectionalValue.X);
		PlayerState = EPlayerState::Walking;
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Adjust the controller's yaw input based on the X axis of the LookAxisValue
		AddControllerYawInput(LookAxisValue.X);
		// Adjust the controller's pitch input based on the Y axis of the LookAxisValue
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void APlayerCharacter::OnSprintUpdate(const FInputActionValue& Value)
{
	constexpr float VelocityThreshold = 0.01f;
	if (CurrentStamina <= 0 || GetVelocity().SizeSquared() <= FMath::Square(VelocityThreshold) || GetCharacterMovement()->IsFalling())
	{
		OnEndSprint(Value);
		return;
	}
	// Set the character's maximum walk speed to the sprint speed modifier
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeedModifier;
	PlayerState = EPlayerState::Sprinting;
}

void APlayerCharacter::OnEndSprint(const FInputActionValue& Value)
{
	// Reset the character's maximum walk speed to the default value
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	PlayerState = EPlayerState::Walking;
}

void APlayerCharacter::RecoverStamina(float DeltaTime)
{
	if (CurrentStamina < MaxStamina && PlayerState != EPlayerState::Sprinting)
	{
		// Increase the current stamina based on the recovery rate and delta time
		CurrentStamina += StaminaRecoveryRate * DeltaTime;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, MaxStamina);
	}
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
		// Interpolate the current shake scale towards the target shake scale
		CurrentShakeScale = FMath::FInterpTo(CurrentShakeScale, TargetShakeScale, DeltaTime, ShakeScaleInterpSpeed);
		// Start the camera shake effect with the specified camera shake class and current shake scale
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(MovementCameraShake,CurrentShakeScale);
	}
}

void APlayerCharacter::OnStartFire(const FInputActionValue& Value) 
{
	if (CurrentWeapon != nullptr && CurrentWeapon->CanFire())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(CurrentWeapon->GetWeaponConfig().HandFireAnimation);
		CurrentWeapon->GetWeaponMesh()->GetAnimInstance()->Montage_Play(CurrentWeapon->GetWeaponConfig().WeaponFireAnimation);
		CurrentWeapon->StartFire();
	}
}

void APlayerCharacter::OnEndFire(const FInputActionValue& Value)
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->StopFire();
	}
}

void APlayerCharacter::Reload(const FInputActionValue& Value)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->OnStartReload();
	}
}

void APlayerCharacter::DecreaseStamina(float DeltaTime)
{
	if (PlayerState == EPlayerState::Sprinting && CurrentStamina > 0)
	{
		// Decrease the current stamina based on the depletion rate and delta time
		CurrentStamina -= StaminaDepletionRate * DeltaTime;
	}
}
