// Copyright Soccertitan 2025


#include "ActionSystem/CrysActionManagerComponent.h"

#include "ActionSystem/CrysAction.h"
#include "Player/CrysPlayerController.h"


UCrysActionManagerComponent::UCrysActionManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	ActionPool.SetNum(MaxCacheSize);
}

void UCrysActionManagerComponent::OnRegister()
{
	Super::OnRegister();
	PlayerController = Cast<ACrysPlayerController>(GetOwner());
}

UCrysAction* UCrysActionManagerComponent::FindAction(FGameplayTag InputTag, int32 Index) const
{
	if (InputTag.IsValid() && ActionMappings.IsValidIndex(Index))
	{
		if (const TObjectPtr<UCrysAction>* ActionPointer = ActionMappings[Index].Find(InputTag))
		{
			return *ActionMappings[Index].Find(InputTag);
		}
	}
	return nullptr;
}

UCrysAction* UCrysActionManagerComponent::FindActionByClass(const TSubclassOf<UCrysAction> ActionClass) const
{
	if (ActionClass)
	{
		for (const TObjectPtr<UCrysAction>& Action : ActionPool)
		{
			if (Action && Action->GetClass() == ActionClass)
			{
				return Action;
			}
		}

		for (const TMap<FGameplayTag, TObjectPtr<UCrysAction>>& Map : ActionMappings)
		{
			for (const TTuple<FGameplayTag, TObjectPtr<UCrysAction>>& Pair : Map)
			{
				if (Pair.Value->GetClass() == ActionClass)
				{
					return Pair.Value;
				}
			}
		}
	}
	return nullptr;
}

bool UCrysActionManagerComponent::CreateActionAndTryActivateOnce(const TSubclassOf<UCrysAction> ActionClass)
{
	if (ActionClass)
	{
		UCrysAction* Action = FindActionByClass(ActionClass);
		if (!Action)
		{
			Action = CreateActionInstance(ActionClass);
			ActionPool[CurrentCacheIndex] = Action;
			CurrentCacheIndex++;
			if (CurrentCacheIndex >= MaxCacheSize)
			{
				CurrentCacheIndex = 0;
			}
		}
		return Action->TryActivateAction();
	}
	return false;
}

bool UCrysActionManagerComponent::TryActivateAction(FGameplayTag InputTag)
{
	return TryActivateActionAtIndex(InputTag, CurrentActionSetIndex);
}

bool UCrysActionManagerComponent::TryActivateActionAtIndex(FGameplayTag InputTag, int32 Index)
{
	if (UCrysAction* Action = FindAction(InputTag, Index))
	{
		return Action->TryActivateAction();
	}
	return false;
}

void UCrysActionManagerComponent::SetAction(const FGameplayTag InputTag, const int32 Index, const TSubclassOf<UCrysAction> ActionClass)
{
	if (!InputTag.IsValid() || Index < 0 || !ActionClass)
	{
		return;
	}
	
	if (!ActionMappings.IsValidIndex(Index))
	{
		ActionMappings.SetNum(Index + 1, EAllowShrinking::No);
	}

	UCrysAction* NewAction = FindActionByClass(ActionClass);
	if (!NewAction)
	{
		NewAction = CreateActionInstance(ActionClass);
	}

	ActionMappings[Index].Add(InputTag, NewAction);
	OnActionMapUpdatedDelegate.Broadcast(NewAction, InputTag, Index);
}

void UCrysActionManagerComponent::ClearAction(FGameplayTag InputTag, int32 Index)
{
	if (!InputTag.IsValid() || Index < 0 || !ActionMappings.IsValidIndex(Index))
	{
		return;
	}
	
	if (ActionMappings[Index].Contains(InputTag))
	{
		ActionMappings[Index].Remove(InputTag);
		OnActionMapUpdatedDelegate.Broadcast(nullptr, InputTag, Index);
	}
}

void UCrysActionManagerComponent::SetCurrentActionSetIndex(int32 Index)
{
	if (Index > 0 && Index != CurrentActionSetIndex)
	{
		CurrentActionSetIndex = Index;
		OnActionSetSelectedDelegate.Broadcast(CurrentActionSetIndex);
	}
}

UCrysAction* UCrysActionManagerComponent::CreateActionInstance(const TSubclassOf<UCrysAction>& ActionClass)
{
	UCrysAction* NewAction = NewObject<UCrysAction>(this, ActionClass);
	NewAction->PlayerController = PlayerController;
	NewAction->InitializeAction();
	return NewAction;
}
