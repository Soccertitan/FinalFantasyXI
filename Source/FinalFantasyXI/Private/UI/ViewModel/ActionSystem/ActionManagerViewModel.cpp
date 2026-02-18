// Copyright Soccertitan 2025


#include "UI/ViewModel/ActionSystem/ActionManagerViewModel.h"

#include "CrysLogChannels.h"
#include "ActionSystem/CrysActionManagerComponent.h"
#include "UI/ViewModel/ActionSystem/ActionItemViewModel.h"

UActionItemViewModel* UActionManagerViewModel::FindOrCreateActionBarItemViewModel(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (UActionItemViewModel* ViewModel : ActionBarItemViewModels)
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

void UActionManagerViewModel::OnInitializeViewModel(APlayerController* PlayerController)
{
	Super::OnInitializeViewModel(PlayerController);
	
	InitActionManager(PlayerController);
}

void UActionManagerViewModel::InitActionManager(APlayerController* PlayerController)
{
	if (ActionManagerComponent)
	{
		return;
	}

	ActionManagerComponent = PlayerController->FindComponentByClass<UCrysActionManagerComponent>();
	if (!ActionManagerComponent)
	{
		UE_LOG(LogCrys, Error, TEXT("%s does not have a CrysActionManagerComponent"), *GetNameSafe(PlayerController));
		return;
	}

	SetActiveActionSetIndex(ActionManagerComponent->GetActionSetIndex());
	ActionManagerComponent->OnActionMapUpdatedDelegate.AddUniqueDynamic(this, &UActionManagerViewModel::OnActionMapUpdated);
	ActionManagerComponent->OnActionSetSelectedDelegate.AddUniqueDynamic(this, &UActionManagerViewModel::SetActiveActionSetIndex);
}

void UActionManagerViewModel::SetActiveActionSetIndex(int32 Index)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(ActiveActionSetIndex, Index))
	{
		if (ActionManagerComponent)
		{
			for (UActionItemViewModel* ViewModel : ActionBarItemViewModels)
			{
				ViewModel->SetAction(ActionManagerComponent->FindAction(ViewModel->GetInputTag(), ActiveActionSetIndex));
			}
		}
	}
}

UActionItemViewModel* UActionManagerViewModel::InternalCreateActionBarItemViewModel(const FGameplayTag& InputTag)
{
	UActionItemViewModel* NewVM = NewObject<UActionItemViewModel>(this);
	NewVM->SetInputTag(InputTag);
	if (ActionManagerComponent)
	{
		NewVM->SetAction(ActionManagerComponent->FindAction(InputTag, ActionManagerComponent->GetActionSetIndex()));
	}
	ActionBarItemViewModels.Add(NewVM);
	return NewVM;
}

void UActionManagerViewModel::OnActionMapUpdated(UCrysAction* Action, const FGameplayTag& InputTag, int32 Index)
{
	if (ActiveActionSetIndex == Index)
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
