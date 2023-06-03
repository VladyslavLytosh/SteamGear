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
	// Maximum number of ammo that character can hold for particular weapon
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 MaxAmmo;
	// Maximum number of rounds that one clip can hold
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 MaxClipAmmo;
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	float TimeBetweenShots;
	// Default data
	FWeaponData()
	{
		bInfinitiveAmmo = false;
		bInfiniteClip = false;
		MaxAmmo = 100;
		MaxClipAmmo = 10;
		TimeBetweenShots = 0.5f;
	}
	
};

UCLASS(Abstract)
class GEARWEAPONS_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	// Function called after components have been initialized
	virtual void PostInitializeComponents() override;

	void StartFire();
	void StopFire();
	void OnStartEquipping();
	void OnEndEquipping();
	
};