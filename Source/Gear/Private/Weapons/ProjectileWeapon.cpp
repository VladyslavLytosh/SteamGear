// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileWeapon.h"
#include "Weapons/Projectile.h"

AProjectileWeapon::AProjectileWeapon()
{
	Muzzle = CreateDefaultSubobject<USceneComponent>("Muzzle");
	Muzzle->SetupAttachment(GetRootComponent());
}

void AProjectileWeapon::Fire()
{
	if (!CanFire())
		return;
	Super::Fire();
	const FVector MuzzleLocation = Muzzle->GetComponentLocation();
	const FRotator MuzzleRotation = Muzzle->GetComponentRotation();

	TObjectPtr<AProjectile> Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileWeaponData.ProjectileClass,MuzzleLocation,MuzzleRotation);
	
	CurrentAmmoInClip -= WeaponConfig.RoundsPerShot;
#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp,Display,TEXT("Current ammo in clip %d"),CurrentAmmoInClip);
#endif
	ApplyRecoil();

}
