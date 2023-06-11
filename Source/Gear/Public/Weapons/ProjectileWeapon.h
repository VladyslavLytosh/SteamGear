// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
USTRUCT()
struct FProjectileWeaponData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly,Category="WeaponConfiguration")
	TSubclassOf<class AProjectile> ProjectileClass; 
	
};

UCLASS()
class GEAR_API AProjectileWeapon : public ABaseWeapon
{
	GENERATED_BODY()	
public:
	AProjectileWeapon();
protected:
	virtual void Fire() override;
	UPROPERTY(EditDefaultsOnly,Category = "Components")
	TObjectPtr<USceneComponent> Muzzle;
	UPROPERTY(EditDefaultsOnly,Category= "Config|Weapon")
	FProjectileWeaponData ProjectileWeaponData;
	
};
