// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StateBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class STATEMACHINE_API UStateBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	bool bCanRepeat = false;
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	FName StateDisplayName;

	virtual void OnEnterState(TObjectPtr<AActor> StateOwner);
	virtual void TickState();
	virtual void OnExitState();
	
};
