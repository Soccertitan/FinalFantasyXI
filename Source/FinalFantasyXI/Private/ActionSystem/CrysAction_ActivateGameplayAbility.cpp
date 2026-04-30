// Copyright Soccertitan 2025


#include "ActionSystem/CrysAction_ActivateGameplayAbility.h"

#include "Ability/CrimGameplayAbility.h"
#include "Input/AbilityInputManagerComponent.h"
#include "Player/CrysPlayerController.h"

bool UCrysAction_ActivateGameplayAbility::CanActivateAction() const
{
	if (Super::CanActivateAction())
	{
		if (!AbilityInputComponent)
		{
			return false;
		}
	}
	
	return true;
}

void UCrysAction_ActivateGameplayAbility::InitializeAction()
{
	Super::InitializeAction();
	
	if (GetPlayerController())
	{
		AbilityInputComponent = GetPlayerController()->FindComponentByClass<UAbilityInputManagerComponent>();
	}
}

void UCrysAction_ActivateGameplayAbility::ActivateAction_Implementation()
{
	Super::ActivateAction_Implementation();
	
	AbilityInputComponent->InputPressed(AbilityClass.Get());
	AbilityInputComponent->InputReleased(AbilityClass.Get());
}
