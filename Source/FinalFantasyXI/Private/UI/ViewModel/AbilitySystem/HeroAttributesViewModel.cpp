// Copyright Soccertitan 2025


#include "UI/ViewModel/AbilitySystem/HeroAttributesViewModel.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "Player/CrysPlayerState.h"

void UHeroAttributesViewModel::OnInitializeViewModel(APlayerController* PlayerController)
{
	Super::OnInitializeViewModel(PlayerController);
	
	AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(PlayerController->GetPlayerState<ACrysPlayerState>());
}
