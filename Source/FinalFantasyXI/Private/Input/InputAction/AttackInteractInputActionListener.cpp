// Copyright Soccertitan 2025


#include "Input/InputAction/AttackInteractInputActionListener.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrimAbilitySystemComponent.h"
#include "CrysBlueprintFunctionLibrary.h"
#include "CrysNativeGameplayTags.h"
#include "GenericTeamAgentInterface.h"
#include "InteractionSystemBlueprintFunctionLibrary.h"
#include "InteractorComponent.h"
#include "TargetingSystemBlueprintFunctionLibrary.h"
#include "TargetingSystemComponent.h"
#include "AbilitySystem/Ability/Combat/AutoAttackManagerComponent.h"
#include "GameFramework/PlayerState.h"

UAttackInteractInputActionListener::UAttackInteractInputActionListener()
{
}

void UAttackInteractInputActionListener::OnInitializeListener()
{
	Super::OnInitializeListener();
	
	if (GetPlayerController())
	{
		GetPlayerController()->OnPossessedPawnChanged.AddUniqueDynamic(this, &UAttackInteractInputActionListener::OnPossessedPawnChanged);
		SetControlledPawn(GetPlayerController()->GetPawn());
	}
}

void UAttackInteractInputActionListener::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	if (TargetingSystemComponent && AutoAttackManagerComponent)
	{
		AActor* TargetedActor = TargetingSystemComponent->GetTargetedActor();
		ETeamAttitude::Type Attitude = UCrysBlueprintFunctionLibrary::GetAttitudeTowardsActor(ControlledPawn, TargetedActor);
		
		if (Attitude == ETeamAttitude::Hostile)
		{
			if (bCombatStance)
			{
				bWaitingForCombatStance = false;
				AutoAttackManagerComponent->StartAutoAttack();
			}
			else if (AbilitySystemComponent)
			{
				bWaitingForCombatStance = true;
				AbilitySystemComponent->TryActivateAbilitiesByTag(Crys::NativeGameplayTag::Ability_Combat_CombatStance.GetTag().GetSingleTagContainer());
			}
			return;
		}
	}
	
	if (InteractorComponent)
	{
		InteractorComponent->BeginInteract();
	}
}

void UAttackInteractInputActionListener::OnInputActionCompleted(const FInputActionValue& Value)
{
	Super::OnInputActionCompleted(Value);
	
	if (InteractorComponent)
	{
		InteractorComponent->EndInteract();
	}
}

void UAttackInteractInputActionListener::OnInputActionCanceled(const FInputActionValue& Value)
{
	Super::OnInputActionCanceled(Value);
	
	if (InteractorComponent)
	{
		InteractorComponent->EndInteract();
	}
}

void UAttackInteractInputActionListener::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	SetControlledPawn(NewPawn);
}

void UAttackInteractInputActionListener::SetControlledPawn(APawn* NewPawn)
{
	TargetingSystemComponent = UTargetingSystemBlueprintFunctionLibrary::GetTargetingSystemComponent(NewPawn);
	InteractorComponent = UInteractionSystemBlueprintFunctionLibrary::GetInteractorComponent(NewPawn);
	ControlledPawn = NewPawn;
	
	if (ControlledPawn)
	{
		APlayerState* PlayerState = ControlledPawn->GetPlayerState();
		AutoAttackManagerComponent = PlayerState ? PlayerState->GetComponentByClass<UAutoAttackManagerComponent>() : nullptr;
	}
	else
	{
		AutoAttackManagerComponent = nullptr;
	}
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->UnregisterGameplayTagEvent(CombatTagDelegateHandle, 
			Crys::NativeGameplayTag::Ability_State_CombatStance, EGameplayTagEventType::NewOrRemoved);
	}
	AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(NewPawn);
	if (AbilitySystemComponent)
	{
		CombatTagDelegateHandle = AbilitySystemComponent->RegisterGameplayTagEvent(Crys::NativeGameplayTag::Ability_State_CombatStance, 
			EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UAttackInteractInputActionListener::OnCombatStanceGameplayTagCountChanged);
		OnCombatStanceGameplayTagCountChanged(Crys::NativeGameplayTag::Ability_State_CombatStance, 
			AbilitySystemComponent->GetGameplayTagCount(Crys::NativeGameplayTag::Ability_State_CombatStance));
	}
}

void UAttackInteractInputActionListener::OnCombatStanceGameplayTagCountChanged(FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		bCombatStance = true;
		
		if (bWaitingForCombatStance && AutoAttackManagerComponent)
		{
			bWaitingForCombatStance = false;
			AutoAttackManagerComponent->StartAutoAttack();
		}
	}
	else
	{
		bCombatStance = false;
	}
}
