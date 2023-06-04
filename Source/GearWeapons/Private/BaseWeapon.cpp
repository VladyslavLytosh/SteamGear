// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CurrentAmmoInClip = WeaponConfig.MaxClipAmmo;
}

void ABaseWeapon::StartFire()
{
	WeaponState = EWeaponState::Firing;
	if (!GetWorld()->GetTimerManager().IsTimerActive(FireRateTimer))
	{
		Fire(); 
		GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &ABaseWeapon::Fire, WeaponConfig.TimeBetweenShots, true);
	}
}

void ABaseWeapon::StopFire()
{
	WeaponState = EWeaponState::Idle;
	GetWorldTimerManager().ClearTimer(FireRateTimer);	
	
}

void ABaseWeapon::OnStartEquipping()
{
	
}

void ABaseWeapon::OnEndEquipping()
{
	
}

bool ABaseWeapon::CanFire()
{
	if (CurrentAmmoInClip > 0 &&
		(WeaponState != EWeaponState::Reloading ||
		WeaponState != EWeaponState::Equipping))
	{
		return true;
	}
	return false;
}

void ABaseWeapon::Fire()
{
#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp,Display,TEXT("Current Ammo in Clip: %d"),CurrentAmmoInClip);
#endif
}



