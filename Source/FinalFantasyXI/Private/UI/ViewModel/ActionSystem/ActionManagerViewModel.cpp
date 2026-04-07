// Copyright Soccertitan 2025


#include "UI/ViewModel/ActionSystem/ActionManagerViewModel.h"

#include "CrysLogChannels.h"
#include "ActionSystem/CrysAction.h"
#include "ActionSystem/CrysActionManagerComponent.h"
#include "System/CrysAssetManager.h"
#include "UI/ViewModel/ActionSystem/ActionViewModel.h"

UActionViewModel* UActionManagerViewModel::FindOrCreateActionViewModel(const FGameplayTag& InputTag, int32 Index)
{
	if (InputTag.IsValid() && Index >= 0)
	{
		if (!ActionViewModelContainers.IsValidIndex(Index))
		{
			ActionViewModelContainers.SetNum(Index + 1, EAllowShrinking::No);
		}
		
		for (const FActionViewModelItem& Item : ActionViewModelContainers[Index].Items)
		{
			if (Item.InputTag == InputTag)
			{
				return Item.ViewModel;
			}
		}
		
		UActionViewModel* ViewModel = InternalCreateActionViewModel(InputTag, Index);
		FActionViewModelItem Item;
		Item.InputTag = InputTag;
		Item.ViewModel = ViewModel;
		ActionViewModelContainers[Index].Items.Add(Item);
		return ViewModel;
	}
	return nullptr;
}

UActionViewModel* UActionManagerViewModel::FindOrCreateActiveActionViewModel(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FActionViewModelItem& Item : ActiveActionViewModels)
		{
			if (Item.InputTag == InputTag)
			{
				return Item.ViewModel;
			}
		}
		
		UActionViewModel* ViewModel = InternalCreateActionViewModel(InputTag, ActiveActionSetIndex);
		FActionViewModelItem Item;
		Item.InputTag = InputTag;
		Item.ViewModel = ViewModel;
		ActiveActionViewModels.Add(Item);
		return ViewModel;
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
		ActionViewModelUpdated.Index = Index;
		if (ActionManagerComponent)
		{
			for (FActionViewModelItem& Item : ActiveActionViewModels)
			{
				ActionViewModelUpdated.InputTag = Item.InputTag;
				Item.ViewModel = InternalCreateActionViewModel(Item.InputTag, Index);
				ActionViewModelUpdated.ViewModel = Item.ViewModel;
				UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(OnActionViewModelUpdated);
			}
		}
		ActionViewModelUpdated = FActionViewModelUpdated();
	}
}

UActionViewModel* UActionManagerViewModel::InternalCreateActionViewModel(const FGameplayTag& InputTag, int32 Index)
{
	UActionViewModel* NewVM = nullptr;
	if (ActionManagerComponent)
	{
		if (UCrysAction* Action = ActionManagerComponent->FindAction(InputTag, Index))
		{
			UCrysAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
				{Action->GetActionViewModel().ToSoftObjectPath()})->WaitUntilComplete();
			NewVM = NewObject<UActionViewModel>(this, Action->GetActionViewModel().Get());
			NewVM->SetAction(Action);
		}
		else
		{
			NewVM = NewObject<UActionViewModel>(this);
		}
	}
	return NewVM;
}

void UActionManagerViewModel::OnActionMapUpdated(UCrysAction* Action, const FGameplayTag& InputTag, int32 Index)
{
	ActionViewModelUpdated.Index = Index;
	ActionViewModelUpdated.InputTag = InputTag;
	ActionViewModelUpdated.ViewModel = InternalCreateActionViewModel(InputTag, Index);
	if (ActiveActionSetIndex == Index)
	{
		for (FActionViewModelItem& Item : ActiveActionViewModels)
		{
			if (Item.InputTag == InputTag)
			{
				Item.ViewModel = ActionViewModelUpdated.ViewModel;
				break;
			}
		}
	}
	
	if (ActionViewModelContainers.IsValidIndex(Index))
	{
		for (FActionViewModelItem& Item : ActionViewModelContainers[Index].Items)
		{
			if (Item.InputTag == InputTag)
			{
				Item.ViewModel = ActionViewModelUpdated.ViewModel;
			}
		}
	}
	
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(OnActionViewModelUpdated);
	ActionViewModelUpdated = FActionViewModelUpdated();
}
