// Copyright Soccertitan 2025


#include "UI/ViewModel/ActionSystem/ActionViewModel.h"

#include "ActionSystem/CrysAction.h"


void UActionViewModel::SetAction(UCrysAction* InAction)
{
	UE_MVVM_SET_PROPERTY_VALUE(Action, InAction);
	if (Action)
	{
		SetActionName(Action->GetActionName());
		SetIcon(Action->GetIcon());
	}
}

void UActionViewModel::SetActionName(FText Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(ActionName, Value);
}

void UActionViewModel::SetIcon(TSoftObjectPtr<UTexture2D> Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(Icon, Value);
}
