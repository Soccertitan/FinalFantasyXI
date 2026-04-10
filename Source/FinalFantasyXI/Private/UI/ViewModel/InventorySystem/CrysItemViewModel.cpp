// Copyright Soccertitan 2025


#include "UI/ViewModel/InventorySystem/CrysItemViewModel.h"

void UCrysItemViewModel::SetUpgradeLevel(int32 Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(UpgradeLevel, Value);
}

void UCrysItemViewModel::SetIsEquipped(bool bValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bEquipped, bValue);
}
