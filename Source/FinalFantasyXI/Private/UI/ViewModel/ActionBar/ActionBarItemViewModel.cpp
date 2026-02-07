// Copyright Soccertitan 2025


#include "UI/ViewModel/ActionBar/ActionBarItemViewModel.h"

#include "UI/ViewModel/AbilitySystem/GameplayAbilityDefinitionViewModel.h"

void UActionBarItemViewModel::SetInputTag(FGameplayTag InValue)
{
	InputTag = InValue;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(InputTag);
}

void UActionBarItemViewModel::SetIsInputPressed(bool InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bInputPressed, InValue);
}

void UActionBarItemViewModel::SetAbilityDefinition(UGameplayAbilityDefinition* AbilityDefinition)
{
	if (!AbilityDefinitionViewModel)
	{
		AbilityDefinitionViewModel = NewObject<UGameplayAbilityDefinitionViewModel>(this);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(AbilityDefinitionViewModel);
	}

	if (AbilityDefinition)
	{
		AbilityDefinitionViewModel->SetAbilityDefinition(AbilityDefinition);
	}
	else
	{
		AbilityDefinitionViewModel->ResetData();
	}
}
