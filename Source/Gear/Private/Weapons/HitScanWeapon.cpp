#include "Weapons/HitScanWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

void AHitScanWeapon::Fire()
{
	Super::Fire();
	
	// Iterate over the number of shots configured for the weapon
	for (int32 ShotIndex = 0; ShotIndex < HitScanWeaponConfig.NumShots; ShotIndex++)
	{
		FHitResult HitResult;
		const FVector LineStart = GetLineStart();
		FVector LineEnd = GetLineEnd();
		
		// Calculate a random offset to simulate shot spread
		const FVector RandomOffset = CalculateRandomOffset(LineEnd);
		LineEnd += RandomOffset;

		GetWorld()->LineTraceSingleByChannel(HitResult, LineStart, LineEnd, ECC_Visibility);

#if !UE_BUILD_SHIPPING
		DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Blue, false, 0.5f, 0, 0);
		DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 20.0f, FColor::Green, false, 0.5f);
#endif

		if (HitResult.GetActor())
		{
			// TODO: Object hit handling
		}
	}
	CurrentAmmoInClip -= WeaponConfig.RoundsPerShot;
#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp,Display,TEXT("Current ammo in clip %d"),CurrentAmmoInClip);
#endif
	ApplyRecoil();
}

FVector AHitScanWeapon::CalculateRandomOffset(const FVector& TargetLocation) const
{
	// Calculate a random deviation within a circular area
	const FVector2D RandomDeviation = FMath::RandPointInCircle(HitScanWeaponConfig.ShotSpreadRadius);
	// Calculate the distance between the weapon and the target location
	const float Distance = FVector::Dist(GetActorLocation(), TargetLocation);

	// Calculate the distance multiplier based on the configured distance spread
	const float DistanceMultiplier = FMath::Lerp(1.f, HitScanWeaponConfig.DistanceSpreadMultiplier, Distance / HitScanWeaponConfig.WeaponRange);
	// Apply the random deviation with the distance multiplier
	return FVector(0.f, RandomDeviation.X * DistanceMultiplier, RandomDeviation.Y * DistanceMultiplier);
}

FVector AHitScanWeapon::GetLineStart() const
{
	return UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();
}

FVector AHitScanWeapon::GetLineEnd() const
{
	const FVector LineStart = GetLineStart();
	const FVector LineDirection = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraRotation().Vector();
	return LineDirection * HitScanWeaponConfig.WeaponRange + LineStart;
}