// Copyright Soccertitan 2025


#include "Input/Action/ActionGameplayAbility.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "InputActionValue.h"
#include "Input/AbilityInputManagerComponent.h"

void UActionGameplayAbility::OnInitializeAction()
{
	Super::OnInitializeAction();
	
	AbilityInputManagerComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilityInputManagerComponent(GetPlayerController());
}

void UActionGameplayAbility::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	if (AbilityInputManagerComponent && bValidAbility)
	{
		if (Value.Get<bool>())
		{
			AbilityInputManagerComponent->InputTagPressed(GetInputTagContainer().First());
		}
		else
		{
			AbilityInputManagerComponent->InputTagReleased(GetInputTagContainer().First());
		}
	}
}

void UActionGameplayAbility::OnInputActionCanceled(const FInputActionValue& Value)
{
	Super::OnInputActionCanceled(Value);
	
	if (AbilityInputManagerComponent && bValidAbility)
	{
		AbilityInputManagerComponent->InputTagReleased(GetInputTagContainer().First());
	}
}

void UActionGameplayAbility::OnInputActionCompleted(const FInputActionValue& Value)
{
	Super::OnInputActionCompleted(Value);
	
	if (AbilityInputManagerComponent && bValidAbility)
	{
		AbilityInputManagerComponent->InputTagReleased(GetInputTagContainer().First());
	}
}

void UActionGameplayAbility::OnInputTagAdded(const FGameplayTag& InputTag)
{
	Super::OnInputTagAdded(InputTag);
	
	if (AbilityInputManagerComponent)
	{
		if (!AbilityClass.IsNull())
		{
			FAbilityInputItem InputItem;
			InputItem.InputTag = InputTag;
			InputItem.GameplayAbilityClass = AbilityClass;
			bValidAbility = true;
			AbilityInputManagerComponent->AddAbilityInputItem(InputItem);
		}
		else
		{
			AbilityInputManagerComponent->RemoveAbilityInputItem(InputTag);
		}
	}
}

void UActionGameplayAbility::OnInputTagRemoved(const FGameplayTag& InputTag)
{
	Super::OnInputTagRemoved(InputTag);
	
	if (AbilityInputManagerComponent)
	{
		AbilityInputManagerComponent->RemoveAbilityInputItem(InputTag);
	}
}
