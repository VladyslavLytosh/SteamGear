// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STATEMACHINE_API UStateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category = "State Machine")
	FString InitialState;
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category = "State Machine")
	TMap<FString, TSubclassOf<class UStateBase>> AvailableStates;
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category = "State Machine|Debug")
	bool bDebug = false;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UStateBase>> StateHistory;
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly, Category = "State Machine|Debug", meta = (ClampMin = "0", ClampMax = "10", UIMin = "0", UIMax = "10"))
	int32 StateHistoryLength;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UStateBase> CurrentState = nullptr;
	
	UPROPERTY()
	TMap<FString, TObjectPtr<UStateBase>> StateMap;

	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void SwitchStateByKey(FString StateKey);
	void SwitchState(TObjectPtr<UStateBase> NewState);
	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void InitStateManager();

private:
	bool bCanTickState = false;
	void InitializesStates();
};
