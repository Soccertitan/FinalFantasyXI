// Copyright Soccertitan 2025


#include "UI/ViewModel/ActionBar/ActionBarViewModel.h"

#include "CrimEnhancedInputComponent.h"
#include "CrysLogChannels.h"
#include "ActionSystem/CrysActionInputActionListener.h"
#include "ActionSystem/CrysActionManagerComponent.h"
#include "UI/ViewModel/ActionBar/ActionBarItemViewModel.h"
#include "UI/ViewModel/ActionBar/InputActionListenerViewModel.h"

void UActionBarViewModel::SetCurrentActionSet(int32 Index)
{
	if (CurrentActionSet != Index && Index >= 0)
	{
		UE_MVVM_SET_PROPERTY_VALUE(CurrentActionSet, Index);
	}
}

UActionBarItemViewModel* UActionBarViewModel::FindOrCreateActionBarItemViewModel(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (UActionBarItemViewModel* ViewModel : ActionBarItemViewModels)
		{
			if (ViewModel->InputTag == InputTag)
			{
				return ViewModel;
			}
		}
		return InternalCreateActionBarItemViewModel(InputTag);
	}
	return nullptr;
}

UInputActionListenerViewModel* UActionBarViewModel::FindOrCreateInputActionListenerViewModel(UInputAction* InputAction)
{
	if (InputAction)
	{
		for (UInputActionListenerViewModel* ViewModel : InputActionListenerViewModels)
		{
			if (ViewModel->GetInputAction() == InputAction)
			{
				return ViewModel;
			}
		}
		return InternalCreateInputActionListenerViewModel(InputAction);
	}
	return nullptr;
}

void UActionBarViewModel::OnInitializeViewModel(APlayerController* PlayerController)
{
	Super::OnInitializeViewModel(PlayerController);
	
	InitActionManager(PlayerController);
	InitEnhancedInput(PlayerController);
}

void UActionBarViewModel::InitActionManager(APlayerController* PlayerController)
{
	ActionManagerComponent = PlayerController->FindComponentByClass<UCrysActionManagerComponent>();
	if (!ActionManagerComponent)
	{
		UE_LOG(LogCrys, Error, TEXT("%s does not have a CrysActionManagerComponent"), *GetNameSafe(PlayerController));
		return;
	}
	
	ActionManagerComponent->OnActionMapUpdatedDelegate.AddUniqueDynamic(this, &UActionBarViewModel::OnActionMapUpdated);
	ActionManagerComponent->OnActionSetSelectedDelegate.AddUniqueDynamic(this, &UActionBarViewModel::OnActionSetSelected);
}

void UActionBarViewModel::InitEnhancedInput(APlayerController* PlayerController)
{
	EnhancedInputComponent = Cast<UCrimEnhancedInputComponent>(PlayerController->InputComponent);
	
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogCrys, Error, TEXT("%s does not have a CrimEnhancedInputComponent"), *GetNameSafe(PlayerController));
		return;
	}
}

UActionBarItemViewModel* UActionBarViewModel::InternalCreateActionBarItemViewModel(const FGameplayTag& InputTag)
{
	UActionBarItemViewModel* NewVM = NewObject<UActionBarItemViewModel>(this);
	NewVM->SetInputTag(InputTag);
	if (ActionManagerComponent)
	{
		NewVM->SetAction(ActionManagerComponent->FindAction(InputTag, ActionManagerComponent->GetCurrentActionSetIndex()));
	}
	ActionBarItemViewModels.Add(NewVM);
	return NewVM;
}

UInputActionListenerViewModel* UActionBarViewModel::InternalCreateInputActionListenerViewModel(UInputAction* InputAction)
{
	UInputActionListenerViewModel* NewVM = NewObject<UInputActionListenerViewModel>(this);
	UCrysInputActionListener* InputActionListener = nullptr;
	if (EnhancedInputComponent)
	{
		InputActionListener = Cast<UCrysInputActionListener>(EnhancedInputComponent->FindListener(InputAction));
	}
	NewVM->InitializeInputActionListener(InputAction, InputActionListener);
	InputActionListenerViewModels.Add(NewVM);
	return NewVM;
}

void UActionBarViewModel::OnActionMapUpdated(UCrysAction* Action, const FGameplayTag& InputTag, int32 Index)
{
	if (CurrentActionSet == Index)
	{
		for (int32 Idx = 0; Idx < ActionBarItemViewModels.Num(); Idx++)
		{
			if (ActionBarItemViewModels[Idx]->InputTag == InputTag)
			{
				ActionBarItemViewModels[Idx]->SetAction(Action);
				break;
			}
		}
	}
}

void UActionBarViewModel::OnActionSetSelected(int32 Index)
{
	UE_MVVM_SET_PROPERTY_VALUE(CurrentActionSet, Index);
	
	for (UActionBarItemViewModel* ViewModel : ActionBarItemViewModels)
	{
		ViewModel->SetAction(ActionManagerComponent->FindAction(ViewModel->GetInputTag(), CurrentActionSet));
	}
}
