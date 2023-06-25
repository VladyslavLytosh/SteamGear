// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/Public/StateManagerComponent.h"

#include "StateBase.h"

// Sets default values for this component's properties
UStateManagerComponent::UStateManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStateManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStateManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStateManagerComponent::SwitchStateByKey(FString StateKey)
{
	TObjectPtr<UStateBase> NewState = StateMap.FindRef(StateKey);

	if (NewState->IsValidLowLevel())
	{
		if (!CurrentState)
		{
			CurrentState = NewState;
		}
		else
		{
			if (CurrentState.GetClass() == NewState.GetClass() && CurrentState == false)
			{
				if (bDebug)
				{
					GEngine->AddOnScreenDebugMessage(INDEX_NONE,3.f,FColor::Red, this->GetOwner()->GetName() + "'s state switch failed. " + CurrentState->StateDisplayName.GetPlainNameString() + " is not repeatable!", true);
				}
			}
			else
			{
				bCanTickState = false;

				CurrentState->OnExitState();

				if (StateHistory.Num() < StateHistoryLength)
				{
					StateHistory.Add(CurrentState);
				}
				else
				{
					StateHistory.RemoveAt(0);
					StateHistory.Add(CurrentState);
				}
				CurrentState = NewState;
			}
		}
		if (CurrentState)
		{
			CurrentState->OnEnterState(GetOwner());
			bCanTickState = true;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE,3.f,FColor::Red, this->GetOwner()->GetName() + "'s state switch failed. " "Invalid state", true);
	}
}

void UStateManagerComponent::SwitchState(TObjectPtr<UStateBase> NewState)
{
}

void UStateManagerComponent::InitStateManager()
{
}

void UStateManagerComponent::InitializesStates()
{
	for (auto It : AvailableStates)
	{
		TObjectPtr<UStateBase> State = NewObject<UStateBase>(this, It.Value);
		StateMap.Add(It.Key, State);
	}
}
