// Copyright Soccertitan 2025


#include "UI/ViewModel/ActionBarViewModel.h"

#include "Input/AbilityInputManagerComponent.h"
#include "Player/Hero/HeroPlayerController.h"
#include "UI/ViewModel/OverlayViewModel.h"
#include "UI/ViewModel/Component/ActionBarItemViewModel.h"

UActionBarItemViewModel* UActionBarViewModel::FindOrCreateActionBarItemViewModel(FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return nullptr;
	}

	for (TObjectPtr<UActionBarItemViewModel>& VM : ActionBarItemViewModels)
	{
		if (VM->GetInputTag() == InputTag)
		{
			return VM;
		}
	}

	UActionBarItemViewModel* NewVM = NewObject<UActionBarItemViewModel>(this);
	NewVM->SetInputTag(InputTag);
	if (AbilityInputManagerComponent)
	{
		NewVM->SetAbilityDefinition(AbilityInputManagerComponent->GetAbilityInputItem(InputTag).AbilityDefinition);
	}
	ActionBarItemViewModels.Add(NewVM);
	return NewVM;
}

void UActionBarViewModel::OnInitializeViewModel(APlayerController* PlayerController)
{
	Super::OnInitializeViewModel(PlayerController);

	if (AHeroPlayerController* HeroPC = Cast<AHeroPlayerController>(PlayerController))
	{
		OnAbilityToggleStateChanged(HeroPC->IsPrimaryAbilityTogglePressed());
		HeroPC->OnPrimaryAbilityToggleStateChangedDelegate.AddUObject(this, &UActionBarViewModel::OnAbilityToggleStateChanged);
		HeroPC->OnInputPressedDelegate.AddUObject(this, &UActionBarViewModel::OnInputPressed);
		HeroPC->OnInputReleasedDelegate.AddUObject(this, &UActionBarViewModel::OnInputReleased);
	}

	AbilityInputManagerComponent = PlayerController->GetComponentByClass<UAbilityInputManagerComponent>();
	if (AbilityInputManagerComponent)
	{
		AbilityInputManagerComponent->OnAbilityInputAddedDelegate.AddUObject(this, &UActionBarViewModel::OnAbilityInputAdded);
		AbilityInputManagerComponent->OnAbilityInputChangedDelegate.AddUObject(this, &UActionBarViewModel::OnAbilityInputChanged);
		AbilityInputManagerComponent->OnAbilityInputRemovedDelegate.AddUObject(this, &UActionBarViewModel::OnAbilityInputRemoved);
	}
}

void UActionBarViewModel::OnAbilityToggleStateChanged(bool bPressed)
{
	UE_MVVM_SET_PROPERTY_VALUE(bAbilityTogglePressed, bPressed);
}

void UActionBarViewModel::OnAbilityInputAdded(UAbilityInputManagerComponent* InputManager, const FAbilityInputItem& AbilityInputItem)
{
	for (const TObjectPtr<UActionBarItemViewModel>& VM : ActionBarItemViewModels)
	{
		if (VM->GetInputTag() == AbilityInputItem.InputTag)
		{
			VM->SetAbilityDefinition(AbilityInputItem.AbilityDefinition);
			return;
		}
	}
}

void UActionBarViewModel::OnAbilityInputChanged(UAbilityInputManagerComponent* InputManager, const FAbilityInputItem& AbilityInputItem)
{
	for (const TObjectPtr<UActionBarItemViewModel>& VM : ActionBarItemViewModels)
	{
		if (VM->GetInputTag() == AbilityInputItem.InputTag)
		{
			VM->SetAbilityDefinition(AbilityInputItem.AbilityDefinition);
			return;
		}
	}
}

void UActionBarViewModel::OnAbilityInputRemoved(UAbilityInputManagerComponent* InputManager, const FAbilityInputItem& AbilityInputItem)
{
	for (const TObjectPtr<UActionBarItemViewModel>& VM : ActionBarItemViewModels)
	{
		if (VM->GetInputTag() == AbilityInputItem.InputTag)
		{
			VM->SetAbilityDefinition(nullptr);
			return;
		}
	}
}

void UActionBarViewModel::OnInputPressed(const FGameplayTag& InputTag)
{
	for (const TObjectPtr<UActionBarItemViewModel>& VM : ActionBarItemViewModels)
	{
		if (VM->GetInputTag() == InputTag)
		{
			VM->SetIsInputPressed(true);
			return;
		}
	}
}

void UActionBarViewModel::OnInputReleased(const FGameplayTag& InputTag)
{
	for (const TObjectPtr<UActionBarItemViewModel>& VM : ActionBarItemViewModels)
	{
		if (VM->GetInputTag() == InputTag)
		{
			VM->SetIsInputPressed(false);
			return;
		}
	}
}
