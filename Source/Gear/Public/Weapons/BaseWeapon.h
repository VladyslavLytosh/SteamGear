// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UENUM()
enum class EWeaponState : uint8
{
	Idle,
	Firing,
	Reloading,
	Equipping
};

USTRUCT()
struct FWeaponData
{
	GENERATED_BODY()
	// If true all weapon ammo will be infinite (reload needed)
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	bool bInfinitiveAmmo;
	// If true, ammo in clip will be infinite (no reload needed)
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	bool bInfiniteClip;
	// Maximum number of rounds that one clip can hold
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 MaxClipAmmo;
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 StartAmmo;
	// Number of rounds expended per shot
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 RoundsPerShot;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Configuration")
	float TimeBetweenShots;
	// When the player shoots with a weapon the camera goes up by a value in the range from MinRecoilNum to MaxRecoilNum 
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Configuration")
	float MinRecoilNum;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Configuration")
	float MaxRecoilNum;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> WeaponReloadAnimation;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> HandReloadAnimation;
	// Default data
	FWeaponData()
	{
		bInfinitiveAmmo = false;
		bInfiniteClip = false;
		MaxClipAmmo = 10;
		TimeBetweenShots = 0.5f;
		MinRecoilNum = -1.f;
		MaxRecoilNum = -1.5f;
		RoundsPerShot = 1;
		StartAmmo = 100;
	}
	
};

UCLASS(Abstract)
class GEAR_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	// Function called after components have been initialized
	virtual void PostInitializeComponents() override;

	virtual void StartFire();
	virtual void StopFire();
	virtual void OnStartEquipping();
	virtual void OnEndEquipping();
	virtual void Reload();
	virtual bool CanReload();

protected:
	// Holds the configuration data for the weapon(can configure in blueprints)
	UPROPERTY(EditDefaultsOnly,Category= "Config|Weapon")
	FWeaponData WeaponConfig;
	EWeaponState WeaponState;
	int32 CurrentAmmoInClip;
	int32 CurrentAmmo;

public:
	EWeaponState GetWeaponState() const;
	void SetWeaponState(const EWeaponState WeaponState);

protected:
	FTimerHandle FireRateTimer;
	// These functions define the internal logic of the weapon and can be overridden by derived classes.
	virtual bool CanFire(); // Checks if the weapon can fire.
	virtual void Fire(); // Performs the actual firing of the weapon
	// A timer handle to control the fire rate of the weapon.
	float LastFireTime;
	void ApplyRecoil() const;
	UPROPERTY(EditDefaultsOnly,Category = "Components")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

public:
	FWeaponData GetWeaponConfig() const;

	TObjectPtr<USkeletalMeshComponent> GetWeaponMesh() const;
};
