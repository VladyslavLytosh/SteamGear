#include "HitScanWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

void AHitScanWeapon::Fire()
{
	if (!CanFire())
		return;

	FHitResult HitResult;
	const FVector LineStart = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();
	FVector LineEnd = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraRotation().Vector() * HitScanWeaponConfig.WeaponRange + LineStart;
	
	const FVector2D RandomDeviation = FMath::RandPointInCircle(HitScanWeaponConfig.ShotSpreadRadius);
	const FVector RandomDirection = LineEnd + FVector(0.f, RandomDeviation.X, RandomDeviation.Y);
	LineEnd = (RandomDirection - LineStart).GetSafeNormal() * HitScanWeaponConfig.WeaponRange + LineStart;

	GetWorld()->LineTraceSingleByChannel(HitResult, LineStart, LineEnd, ECC_Visibility);

#if !UE_BUILD_SHIPPING
	DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Blue, false, 0.5f,0,0.25);
	DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 20.0f, FColor::Green, false, 0.5f);
#endif

	if (HitResult.GetActor())
	{
		// TODO : Object hit handling

	}

	CurrentAmmoInClip--;
	ApplyRecoil();
}