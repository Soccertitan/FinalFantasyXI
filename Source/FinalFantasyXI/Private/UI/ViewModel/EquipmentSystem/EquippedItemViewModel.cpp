// Copyright Soccertitan 2025


#include "UI/ViewModel/EquipmentSystem/EquippedItemViewModel.h"

void UEquippedItemViewModel::SetEquipSlot(const FGameplayTag& InValue)
{
	EquipSlot = InValue;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(EquipSlot);
	// EquipSlotName = EquipSlot.GetTagLeafName();
	// UE_MVVM_SET_PROPERTY_VALUE();
}

void UEquippedItemViewModel::SetItemInstanceViewModel(UItemInstanceViewModel* InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(ItemInstanceViewModel, InValue);
}
