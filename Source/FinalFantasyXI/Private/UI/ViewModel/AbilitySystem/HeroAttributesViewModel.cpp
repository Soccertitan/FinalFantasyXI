// Copyright Soccertitan 2025


#include "UI/ViewModel/AbilitySystem/HeroAttributesViewModel.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "Player/CrysPlayerState.h"

void UHeroAttributesViewModel::InitializeViewModel(APlayerController* PlayerController)
{
	Super::InitializeViewModel(PlayerController);
	
	if (UCrimAbilitySystemComponent* AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(PlayerController->GetPlayerState<ACrysPlayerState>()))
	{
		OnAbilitySystemComponentRetrieved(AbilitySystemComponent);
	}
}
