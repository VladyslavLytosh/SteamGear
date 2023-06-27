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
		const TObjectPtr<USkeletalMeshComponent> HandsMesh = Cast<ACharacter>(GetOwner())->GetMesh();
		const float AnimationTime = HandsMesh->GetAnimInstance()->Montage_Play(WeaponConfig.HandReloadAnimation);
		GetWeaponMesh()->GetAnimInstance()->Montage_Play(WeaponConfig.WeaponReloadAnimation);
		GetWorldTimerManager().SetTimer(ReloadTimer, this, &ABaseWeapon::OnEndReload,AnimationTime, false);
		SetWeaponState(EWeaponState::Reloading);
	}
}

void ABaseWeapon::OnEndReload()
{
	Reload();
	WeaponState = EWeaponState::Idle;
}


EWeaponState ABaseWeapon::GetWeaponState() const
{
	return WeaponState;
}

void ABaseWeapon::SetWeaponState(const EWeaponState _WeaponState)
{
	this->WeaponState = _WeaponState;
}

bool ABaseWeapon::CanFire()
{
	// Checks if the weapon can currently fire based фффon the ammo in the clip and the current weapon state
	return CurrentAmmoInClip > 0 &&
		(WeaponState != EWeaponState::Reloading || WeaponState != EWeaponState::Equipping);
}


void ABaseWeapon::Fire()
{
}

bool ABaseWeapon::CanReload()
{
	return CurrentAmmoInClip < WeaponConfig.MaxClipAmmo && WeaponState != EWeaponState::Reloading;
}


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

FWeaponData ABaseWeapon::GetWeaponConfig() const
{
	return WeaponConfig;
}

TObjectPtr<USkeletalMeshComponent> ABaseWeapon::GetWeaponMesh() const
{
	return WeaponMesh;
}



