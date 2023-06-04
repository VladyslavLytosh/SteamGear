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
	UPROPERTY(EditDefaultsOnly,Category="WeaponStats")
	float WeaponRange;
};

UCLASS()
class GEARWEAPONS_API AHitScanWeapon : public ABaseWeapon
{
	GENERATED_BODY()
protected:
	virtual void Fire() override;
	UPROPERTY(EditDefaultsOnly,Category= "Config|Weapon")
	FHitScanWeaponData HitScanWeaponConfig;
};
