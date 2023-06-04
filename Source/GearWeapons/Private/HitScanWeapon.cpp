// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"


void AHitScanWeapon::Fire()
{
	Super::Fire();
	FHitResult HitResult;
	FVector LineStart = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();;
	FVector LineEnd = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraRotation().Vector();;
	GetWorld()->LineTraceSingleByChannel(HitResult,LineStart,LineEnd * HitScanWeaponConfig.WeaponRange + LineStart,ECC_Visibility);
#if !UE_BUILD_SHIPPING
	DrawDebugLine(GetWorld(),LineStart,LineEnd * HitScanWeaponConfig.WeaponRange + LineStart,FColor::Red,true,2);
#endif
	if (HitResult.GetActor())
	{
		
	}
}
