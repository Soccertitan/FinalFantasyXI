// Copyright Soccertitan 2025


#include "UI/ViewModel/EquipmentSystem/EquippedItemViewModel.h"

#include "UI/ViewModel/CrysGameplayTagViewModel.h"

void UEquippedItemViewModel::SetEquipSlot(const FGameplayTag& InValue)
{
	if (EquipSlot != InValue)
	{
		EquipSlot = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(EquipSlot);
		
		UCrysGameplayTagViewModel* NewVM = NewObject<UCrysGameplayTagViewModel>(this);
		NewVM->SetGameplayTag(EquipSlot);
		SetTagRelationshipItem(NewVM);
	}
}

void UEquippedItemViewModel::SetItemInstanceViewModel(UItemInstanceViewModel* InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(ItemInstanceViewModel, InValue);
}

void UEquippedItemViewModel::SetTagRelationshipItem(UCrysGameplayTagViewModel* InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(TagRelationshipItem, InValue);
}
