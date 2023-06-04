// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "DrawDebugHelpers.h"



void AHitScanWeapon::Fire()
{
	Super::Fire();
	FHitResult HitResult;
	FVector LineStart;
	FRotator LineEnd;
	GetActorEyesViewPoint(LineStart,LineEnd);
	GetWorld()->LineTraceSingleByChannel(HitResult,LineStart,LineEnd.Vector() + HitScanWeaponConfig.WeaponRange,ECC_Visibility);
#if !UE_BUILD_SHIPPING
	DrawDebugLine(GetWorld(),LineStart,LineEnd.Vector() + HitScanWeaponConfig.WeaponRange,FColor::Red,true,2);
#endif
	if (HitResult.GetActor())
	{
		
	}
}
