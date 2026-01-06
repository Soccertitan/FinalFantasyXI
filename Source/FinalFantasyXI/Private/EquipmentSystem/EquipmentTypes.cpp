// Copyright Soccertitan 2025


#include "EquipmentSystem/EquipmentTypes.h"

#include "EquipmentSystem/EquipmentManagerComponent.h"


void FEquippedItem::PostReplicatedAdd(const struct FEquippedItemsContainer& Container)
{
	Container.Owner->OnItemEquipped(*this);
}

void FEquippedItem::PreReplicatedRemove(const FEquippedItemsContainer& Container)
{
	Container.Owner->OnItemUnequipped(*this);
}

bool FEquippedItem::IsValid() const
{
	return EquipSlot.IsValid();
}

FEquippedItem* FEquippedItemsContainer::FindItemByEquipSlot(const FGameplayTag& EquipSlot) const
{
	for (const FEquippedItem& EquippedItem : Items)
	{
		if (EquippedItem.EquipSlot == EquipSlot)
		{
			return const_cast<FEquippedItem*>(&EquippedItem);
		}
	}
	return nullptr;
}

FEquippedItem* FEquippedItemsContainer::FindItemByItemGuid(const FGuid& ItemGuid) const
{
	for (const FEquippedItem& EquippedItem : Items)
	{
		if (EquippedItem.ItemGuid == ItemGuid)
		{
			return const_cast<FEquippedItem*>(&EquippedItem);
		}
	}
	return nullptr;
}
