// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BaseWeapon.h"

#include "GameFramework/Character.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));

}

void ABaseWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CurrentAmmoInClip = WeaponConfig.MaxClipAmmo;
	CurrentAmmo = WeaponConfig.StartAmmo;
}

void ABaseWeapon::StartFire()
{
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
	if (WeaponState != EWeaponState::Reloading && WeaponState != EWeaponState::Equipping)
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

void ABaseWeapon::Reload()
{
	const int32 AmmoNeeded = WeaponConfig.MaxClipAmmo - CurrentAmmoInClip;

	if (CurrentAmmo < AmmoNeeded)
	{
		CurrentAmmoInClip += CurrentAmmo;
		CurrentAmmo = 0;
	}
	else
	{
		CurrentAmmo -= AmmoNeeded;
		CurrentAmmoInClip += AmmoNeeded;
	}
#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp,Display,TEXT("Current ammo in clip %d"),CurrentAmmoInClip);
	UE_LOG(LogTemp,Display,TEXT("Current ammo  %d"),CurrentAmmo);
#endif
	
}

void ABaseWeapon::OnStartReload()
{
	if (CanReload())
	{
		WeaponState = EWeaponState::Reloading;
		const TObjectPtr<USkeletalMeshComponent> HandMesh = Cast<ACharacter>(GetOwner())->GetMesh();
		const float AnimationTime = HandMesh->GetAnimInstance()->Montage_Play(WeaponConfig.HandReloadAnimation);
		WeaponMesh->GetAnimInstance()->Montage_Play(WeaponConfig.WeaponReloadAnimation);
		GetWorldTimerManager().SetTimer(ReloadTimer, this, &ABaseWeapon::OnEndReload,AnimationTime, false);
	}
}

void ABaseWeapon::OnEndReload()
{
	Reload();
	WeaponState = EWeaponState::Idle;
}

bool ABaseWeapon::CanFire()
{
	// Checks if the weapon can currently fire based on the ammo in the clip and the current weapon state
	return CurrentAmmoInClip > 0 &&
		(WeaponState != EWeaponState::Reloading && WeaponState != EWeaponState::Equipping);
}


void ABaseWeapon::Fire()
{
	WeaponState = EWeaponState::Firing;
	const TObjectPtr<USkeletalMeshComponent> HandMesh = Cast<ACharacter>(GetOwner())->GetMesh();
	HandMesh->GetAnimInstance()->Montage_Play(WeaponConfig.HandFireAnimation);
	WeaponMesh->GetAnimInstance()->Montage_Play(WeaponConfig.WeaponFireAnimation);
}

bool ABaseWeapon::CanReload()
{
	return CurrentAmmoInClip < WeaponConfig.MaxClipAmmo && CurrentAmmo > 0 && (WeaponState != EWeaponState::Reloading
		&& WeaponState != EWeaponState::Firing && WeaponState != EWeaponState::Equipping);
}
