// Copyright Soccertitan 2025


#include "UI/ViewModel/ActionSystem/ActionSlotViewModel.h"

void UActionSlotViewModel::SetActionViewModel(UActionViewModel* Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(ActionViewModel, Value);
}
