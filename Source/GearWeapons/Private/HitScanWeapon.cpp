#include "HitScanWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

void AHitScanWeapon::Fire()
{
	if (!CanFire())
		return;

	for (int32 ShotIndex = 0; ShotIndex < HitScanWeaponConfig.NumShots; ShotIndex++)
	{
		FHitResult HitResult;
		const FVector LineStart = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();
		FVector LineEnd = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraRotation().Vector() * HitScanWeaponConfig.WeaponRange + LineStart;

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
	CurrentAmmoInClip--;
	ApplyRecoil();
}

FVector AHitScanWeapon::CalculateRandomOffset(const FVector& TargetLocation) const
{
	const FVector2D RandomDeviation = FMath::RandPointInCircle(HitScanWeaponConfig.ShotSpreadRadius);
	const float Distance = FVector::Dist(GetActorLocation(), TargetLocation);


	const float DistanceMultiplier = FMath::Lerp(HitScanWeaponConfig.DistanceSpreadMultiplier, 1.f, Distance / HitScanWeaponConfig.WeaponRange);

	return FVector(0.f, RandomDeviation.X * DistanceMultiplier, RandomDeviation.Y * DistanceMultiplier);
}