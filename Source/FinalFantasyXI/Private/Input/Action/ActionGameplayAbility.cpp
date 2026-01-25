// Copyright Soccertitan 2025


#include "Input/Action/ActionGameplayAbility.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "InputActionValue.h"
#include "Input/AbilityInputManagerComponent.h"

void UActionGameplayAbility::OnInitializeAction()
{
	Super::OnInitializeAction();
	
	AbilityInputManagerComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilityInputManagerComponent(GetPlayerController());
	if (AbilityInputManagerComponent)
	{
		if (!AbilityClass.IsNull())
		{
			FAbilityInputItem InputItem;
			InputItem.InputTag = GetInputTag();
			InputItem.GameplayAbilityClass = AbilityClass;
			bValidAbility = true;
			AbilityInputManagerComponent->AddAbilityInputItem(InputItem);
		}
		else
		{
			AbilityInputManagerComponent->RemoveAbilityInputItem(GetInputTag());
		}
	}
}

void UActionGameplayAbility::OnResetAction()
{
	Super::OnResetAction();
	
	AbilityInputManagerComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilityInputManagerComponent(GetPlayerController());
	if (AbilityInputManagerComponent)
	{
		AbilityInputManagerComponent->RemoveAbilityInputItem(GetInputTag());
	}
}

void UActionGameplayAbility::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	if (AbilityInputManagerComponent && bValidAbility)
	{
		if (Value.Get<bool>())
		{
			AbilityInputManagerComponent->InputTagPressed(GetInputTag());
		}
		else
		{
			AbilityInputManagerComponent->InputTagReleased(GetInputTag());
		}
	}
}
