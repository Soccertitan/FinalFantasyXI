// Copyright Soccertitan 2025


#include "UI/ViewModel/OverlayViewModel.h"

#include "CrimAbilitySystemComponent.h"
#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "HeroSystem/HeroManagerComponent.h"
#include "HeroSystem/HeroSystemBlueprintFunctionLibrary.h"
#include "Player/CrysPlayerState.h"
#include "UI/ViewModel/Component/AttributeFractionViewModel.h"
#include "UI/ViewModel/Component/AttributeViewModel.h"
#include "UI/ViewModel/Component/HeroClassViewModel.h"

void UOverlayViewModel::OnInitializeViewModel(APlayerController* PlayerController)
{
	Super::OnInitializeViewModel(PlayerController);

	AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(PlayerController->GetPlayerState<ACrysPlayerState>());
	
	HeroManagerComponent = UHeroSystemBlueprintFunctionLibrary::GetHeroManagerComponent(PlayerController->GetPlayerState<ACrysPlayerState>());
	if (HeroManagerComponent)
	{
		OnHeroClassChanged(HeroManagerComponent);
		
		HeroManagerComponent->OnHeroClassProgressUpdatedDelegate.AddUniqueDynamic(this, &UOverlayViewModel::OnHeroClassProgressUpdated);
		HeroManagerComponent->OnHeroClassChangedDelegate.AddUniqueDynamic(this, &UOverlayViewModel::OnHeroClassChanged);
	}
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

void UOverlayViewModel::OnHeroClassProgressUpdated(UHeroManagerComponent* InHeroManagerComponent, const FHeroClassProgressItem& HeroClassProgressItem)
{
	if (HeroClassViewModel && HeroClassViewModel->GetHeroClass()->HeroClassTag.MatchesTagExact(HeroClassProgressItem.HeroClassTag))
	{
		HeroClassViewModel->SetHeroClassProgressItem(HeroClassProgressItem);
	}
}

void UOverlayViewModel::OnHeroClassChanged(UHeroManagerComponent* InHeroManagerComponent)
{
	if (UHeroClassDefinition* HeroClass = InHeroManagerComponent->GetHeroClass())
	{
		HeroClassViewModel = NewObject<UHeroClassViewModel>(this, UHeroClassViewModel::StaticClass());
		const FHeroClassProgressItem ProgressItem = HeroManagerComponent->FindHeroClassProgressItem(HeroClass->HeroClassTag);
		HeroClassViewModel->SetHeroClassAndProgress(HeroClass, ProgressItem);
		HeroClassViewModel->SetIsCurrentHeroClass(true);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(HeroClassViewModel);
	}
}
