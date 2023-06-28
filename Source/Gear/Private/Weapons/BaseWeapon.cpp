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
	ResetAmmo();
}

void ABaseWeapon::StartFire()
{
	if (GetWorldTimerManager().IsTimerActive(FireRateTimer))
		return;
	
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
	const int32 AmmoNeeded = FMath::Max(0, WeaponConfig.MaxClipAmmo - CurrentAmmoInClip);
	const int32 AmmoAvailable = FMath::Min(CurrentAmmo, AmmoNeeded);

	CurrentAmmoInClip += AmmoAvailable;
	CurrentAmmo -= AmmoAvailable;
#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp,Display,TEXT("Current ammo in clip %d"),CurrentAmmoInClip);
	UE_LOG(LogTemp,Display,TEXT("Current ammo  %d"),CurrentAmmo);
#endif
	
}

void ABaseWeapon::OnStartReload()
{
	if (!CanReload())
		return;
	StartReloadingAnimation();
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
	const ACharacter* Character = GetOwningCharacter();
	if (!Character)
		return;
	const USkeletalMeshComponent* HandMesh = Character->GetMesh();
	if (!HandMesh)
		return;
	HandMesh->GetAnimInstance()->Montage_Play(WeaponConfig.HandFireAnimation);
	WeaponMesh->GetAnimInstance()->Montage_Play(WeaponConfig.WeaponFireAnimation);
}

bool ABaseWeapon::CanReload()
{
	return CurrentAmmoInClip < WeaponConfig.MaxClipAmmo && CurrentAmmo > 0 && (WeaponState != EWeaponState::Reloading
		&& WeaponState != EWeaponState::Firing && WeaponState != EWeaponState::Equipping);
}

void ABaseWeapon::StartReloadingAnimation()
{
	WeaponState = EWeaponState::Reloading;

	const ACharacter* Character = GetOwningCharacter();
	if (!Character)
		return;

	const USkeletalMeshComponent* HandMesh = Character->GetMesh();
	if (!HandMesh)
		return;

	const float AnimationTime = HandMesh->GetAnimInstance()->Montage_Play(WeaponConfig.HandReloadAnimation, WeaponConfig.ReloadTime) / WeaponConfig.ReloadTime;
	WeaponMesh->GetAnimInstance()->Montage_Play(WeaponConfig.WeaponReloadAnimation, WeaponConfig.ReloadTime);
	GetWorldTimerManager().SetTimer(ReloadTimer, this, &ABaseWeapon::OnEndReload, AnimationTime, false);
}

const ACharacter* ABaseWeapon::GetOwningCharacter() const
{
	const UObject* WeaponOwner = GetOwner();
	return WeaponOwner && WeaponOwner->IsA<ACharacter>() ? Cast<ACharacter>(WeaponOwner) : nullptr;
}

void ABaseWeapon::ResetAmmo()
{
	CurrentAmmoInClip = WeaponConfig.MaxClipAmmo;
	CurrentAmmo = WeaponConfig.StartAmmo;
}