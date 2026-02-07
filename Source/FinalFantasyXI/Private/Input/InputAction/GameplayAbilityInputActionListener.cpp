// Copyright Soccertitan 2025


#include "Input/InputAction/GameplayAbilityInputActionListener.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "InputActionValue.h"
#include "Input/AbilityInputManagerComponent.h"

void UGameplayAbilityInputActionListener::OnInitializeListener()
{
	Super::OnInitializeListener();
	
	AbilityInputManagerComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilityInputManagerComponent(GetPlayerController());
}

void UGameplayAbilityInputActionListener::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	if (AbilityInputManagerComponent && bValidAbility)
	{
		if (Value.Get<bool>())
		{
			AbilityInputManagerComponent->InputPressed(AbilityClass);
		}
		else
		{
			AbilityInputManagerComponent->InputReleased(AbilityClass);
		}
	}
}

void UGameplayAbilityInputActionListener::OnInputActionCanceled(const FInputActionValue& Value)
{
	Super::OnInputActionCanceled(Value);
	
	if (AbilityInputManagerComponent && bValidAbility)
	{
		AbilityInputManagerComponent->InputReleased(AbilityClass);
	}
}

void UGameplayAbilityInputActionListener::OnInputActionCompleted(const FInputActionValue& Value)
{
	Super::OnInputActionCompleted(Value);
	
	if (AbilityInputManagerComponent && bValidAbility)
	{
		AbilityInputManagerComponent->InputReleased(AbilityClass);
	}
}
