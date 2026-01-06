// Copyright Soccertitan 2025


#include "UI/ViewModel/InventorySystem/Filter/ItemViewModelFilter_EquipableItems.h"

#include "EquipmentSystem/EquipmentManagerComponent.h"
#include "UI/ViewModel/ItemInstanceViewModel.h"

bool UItemViewModelFilter_EquipableItems::ShouldBeginFilter(const UObject* Context, TArray<UItemInstanceViewModel*>& ItemViewModels) const
{
	if (!Super::ShouldBeginFilter(Context, ItemViewModels))
	{
		return false;
	}

	if (!EquipSlot.IsValid())
	{
		return false;
	}

	if (!Cast<UEquipmentManagerComponent>(Context))
	{
		return false;
	}

	return true;
}

bool UItemViewModelFilter_EquipableItems::DoesItemInstanceViewModelPassFilter(const UObject* Context, UItemInstanceViewModel* ItemViewModel) const
{
	if (!Super::DoesItemInstanceViewModelPassFilter(Context, ItemViewModel))
	{
		return false;
	}

	if (!Cast<UEquipmentManagerComponent>(Context)->CanEquipItem(EquipSlot, ItemViewModel->GetItem()))
	{
		return false;
	}

	return true;
}
