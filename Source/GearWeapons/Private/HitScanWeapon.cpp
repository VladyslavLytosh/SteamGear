// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"


void AHitScanWeapon::Fire()
{
	if (!CanFire())
		return;
	
	FHitResult HitResult;
	FVector LineStart = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();;
	FVector LineEnd = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraRotation().Vector();;
	GetWorld()->LineTraceSingleByChannel(HitResult,LineStart,LineEnd * HitScanWeaponConfig.WeaponRange + LineStart,ECC_Visibility);
#if !UE_BUILD_SHIPPING
	DrawDebugLine(GetWorld(),LineStart,LineEnd * HitScanWeaponConfig.WeaponRange + LineStart,FColor::Red,false,0.5f);
	DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Green, false, 0.5f);
#endif
	if (HitResult.GetActor())
	{
		
	}
	CurrentAmmoInClip--;

	Super::Fire();
}
