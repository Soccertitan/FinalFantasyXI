// Copyright Soccertitan 2025


#include "UI/ViewModel/ActionBar/ActionBarItemViewModel.h"

#include "ActionSystem/CrysAction.h"
#include "System/CrysAssetManager.h"

void UActionBarItemViewModel::SetInputTag(const FGameplayTag& InInputTag)
{
	InputTag = InInputTag;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(InputTag);
}

void UActionBarItemViewModel::SetAction(UCrysAction* InAction)
{
	Action = InAction;
	if (Action)
	{
		UE_MVVM_SET_PROPERTY_VALUE(ActionName, Action->GetActionName());
		if (!Action->GetIcon().Get())
		{
			ActionObjectsStreamableHandle = UCrysAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
				{Action->GetIcon().ToSoftObjectPath()}, 
				FStreamableDelegate::CreateUObject(this, &UActionBarItemViewModel::OnActionObjectsLoaded)
				);
		}
		else
		{
			UE_MVVM_SET_PROPERTY_VALUE(Icon, Action->GetIcon().Get());
		}
	}
	else
	{
		UE_MVVM_SET_PROPERTY_VALUE(ActionName, FText());
		UE_MVVM_SET_PROPERTY_VALUE(Icon, nullptr);
	}
}

void UActionBarItemViewModel::OnActionObjectsLoaded()
{
	if (Action)
	{
		UE_MVVM_SET_PROPERTY_VALUE(Icon, Action->GetIcon().Get());
	}
	ActionObjectsStreamableHandle.Reset();
}
