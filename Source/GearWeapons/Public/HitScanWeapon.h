// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "HitScanWeapon.generated.h"

/**
 * 
 */
USTRUCT()
struct FHitScanWeaponData
{
	GENERATED_BODY()
	// The maximum range of the hitscan weapon
	UPROPERTY(EditDefaultsOnly,Category="WeaponStats")
	float WeaponRange;
	// The spread radius of shots fired by the hitscan weapon (During firing, a random point is selected from a circle with a radius of ShotSpreadRadius value)
	UPROPERTY(EditDefaultsOnly,Category="WeaponStats")
	float ShotSpreadRadius;
	// The number of shots fired by the hitscan weapon (Mostly needed to simulate a shotgun)
	UPROPERTY(EditDefaultsOnly,Category="WeaponStats")
	float NumShots;
	// The spread multiplier based on the distance to the target
	UPROPERTY(EditDefaultsOnly,Category="WeaponStats")
	float DistanceSpreadMultiplier;
	FHitScanWeaponData()
	{
		WeaponRange = 2500;
		ShotSpreadRadius = 120.f;
		NumShots = 1;
		DistanceSpreadMultiplier = 0.5;
	}
};

UCLASS()
class GEARWEAPONS_API AHitScanWeapon : public ABaseWeapon
{
	GENERATED_BODY()
protected:
	virtual void Fire() override;
	FVector CalculateRandomOffset(const FVector& TargetLocation) const;
	// Returns the start point of the line trace for hitscan weapon shots
	FVector GetLineStart() const;
	// Returns the end point of the line trace for hitscan weapon shots
	FVector GetLineEnd() const;
	// Holds the configuration data specific to the hitscan weapon.
	UPROPERTY(EditDefaultsOnly,Category= "Config|Weapon")
	FHitScanWeaponData HitScanWeaponConfig;
};
