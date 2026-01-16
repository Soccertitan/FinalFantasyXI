// Copyright Soccertitan 2025


#include "UI/ViewModel/OverlayViewModel.h"

#include "CrimAbilitySystemComponent.h"
#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "Player/CrysPlayerState.h"
#include "UI/ViewModel/Component/AttributeFractionViewModel.h"
#include "UI/ViewModel/Component/AttributeViewModel.h"
#include "UI/ViewModel/Component/HeroJobViewModel.h"

void UOverlayViewModel::OnInitializeViewModel(APlayerController* PlayerController)
{
	Super::OnInitializeViewModel(PlayerController);

	AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(PlayerController->GetPlayerState<ACrysPlayerState>());
}

UAttributeFractionViewModel* UOverlayViewModel::CreateAttributeFractionViewModel(const FGameplayTag NumeratorAttributeTag,
	FGameplayTag DenominatorAttributeTag)
{
	UAttributeFractionViewModel* NewVM = NewObject<UAttributeFractionViewModel>(this, UAttributeFractionViewModel::StaticClass());
	NewVM->SetAttributesWithASC(AbilitySystemComponent, NumeratorAttributeTag, DenominatorAttributeTag);
	return NewVM;
}

UAttributeViewModel* UOverlayViewModel::CreateAttributeViewModel(const FGameplayTag AttributeTag)
{
	UAttributeViewModel* NewVM = NewObject<UAttributeViewModel>(this, UAttributeViewModel::StaticClass());
	NewVM->SetAttribute(AttributeTag, AbilitySystemComponent);
	return NewVM;
}
