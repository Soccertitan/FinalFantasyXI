// Copyright Soccertitan 2025


#include "UI/ViewModel/InventorySystem/Filter/ItemInstanceViewModelFilter_EquipableItems.h"

#include "EquipmentSystem/EquipmentManagerComponent.h"
#include "UI/ViewModel/ItemInstanceViewModel.h"

bool UItemInstanceViewModelFilter_EquipableItems::ShouldBeginFilter(const UObject* Context, TArray<UItemInstanceViewModel*>& ViewModels) const
{
	if (!Super::ShouldBeginFilter(Context, ViewModels))
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

bool UItemInstanceViewModelFilter_EquipableItems::DoesItemInstanceViewModelPassFilter(const UObject* Context, UItemInstanceViewModel* ViewModel) const
{
	if (!Super::DoesItemInstanceViewModelPassFilter(Context, ViewModel))
	{
		return false;
	}

	if (!Cast<UEquipmentManagerComponent>(Context)->CanEquipItem(EquipSlot, ViewModel->GetItemInstance().GetItem()))
	{
		return false;
	}

	return true;
}
