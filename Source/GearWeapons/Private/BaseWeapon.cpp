// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CurrentAmmoInClip = WeaponConfig.MaxClipAmmo;
}

void ABaseWeapon::StartFire()
{
	WeaponState = EWeaponState::Firing;
	// Checks if enough time has passed since the last shot, so we can make delay between single shots
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastFireTime >= WeaponConfig.TimeBetweenShots)
	{
		Fire();
		LastFireTime = CurrentTime;
	}
	// Sets up a timer to repeatedly call the Fire() function based on the configured time between shots (worked if player hold fire button)
	GetWorldTimerManager().SetTimer(FireRateTimer, this, &ABaseWeapon::Fire, WeaponConfig.TimeBetweenShots, true);
}

void ABaseWeapon::StopFire()
{
	WeaponState = EWeaponState::Idle;
	GetWorldTimerManager().ClearTimer(FireRateTimer);	
	
}

void ABaseWeapon::OnStartEquipping()
{
	// TODO : Equipping
}

void ABaseWeapon::OnEndEquipping()
{
	// TODO : Equipping
}

bool ABaseWeapon::CanFire()
{
	// Checks if the weapon can currently fire based on the ammo in the clip and the current weapon state
	return CurrentAmmoInClip > 0 &&
		(WeaponState != EWeaponState::Reloading || WeaponState != EWeaponState::Equipping);
}


void ABaseWeapon::Fire() { }


void ABaseWeapon::ApplyRecoil() const
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController != nullptr)
	{
		// Generates a random recoil pitch within the specified range
		const float RecoilPitch = FMath::FRandRange(WeaponConfig.MinRecoilNum, WeaponConfig.MaxRecoilNum);
		PlayerController->AddPitchInput(RecoilPitch);
	}
}



