// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class GEAR_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	// Function called after components have been initialized
	virtual void PostInitializeComponents() override;
private:
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<class UProjectileMovementComponent> MovementComponent;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp,AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
