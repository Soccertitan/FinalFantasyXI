// Copyright Soccertitan 2025


#include "EquipmentSystem/ItemFragment_Equipment.h"

#include "CrysGameplayTags.h"


UEquipmentManagerComponent* FItemShard_Equipment::GetEquipmentManagerComponent() const
{
	return EquipmentManagerComponent.Get();
}

bool FItemShard_Equipment::IsMatching(const TInstancedStruct<FItemShard>& Shard) const
{
	if (FItemShard::IsMatching(Shard))
	{
		if (const FItemShard_Equipment* Ptr = Shard.GetPtr<FItemShard_Equipment>())
		{
			if (Ptr->Level == Level)
			{
				return true;
			}
		}	
	}
	return false;
}

FItemFragment_Equipment::FItemFragment_Equipment()
{
	EquipmentShard.InitializeAsScriptStruct(FItemShard_Equipment::StaticStruct());
}

void FItemFragment_Equipment::SetDefaultValues(TInstancedStruct<FItem>& Item) const
{
	TInstancedStruct<FItemShard> NewShard;
	NewShard.InitializeAsScriptStruct(EquipmentShard.GetScriptStruct());
	AddItemShard(Item, NewShard);
}

void FItemFragment_Equipment::PostSerialize(const FArchive& Ar)
{
	// Ensure that the EquipSlot tags are not also in the BlockedEquipSlots.
	BlockEquipSlots.RemoveTags(EquipSlots);

	if (!StaticStruct()->IsChildOf(FItemFragment_Weapon::StaticStruct()))
	{
		EquipSlots.RemoveTag(FCrysGameplayTags::Get().EquipSlot_MainHand);
	}
}

void FItemFragment_Weapon::PostSerialize(const FArchive& Ar)
{
	Super::PostSerialize(Ar);

	// Ensure weapons can only have the MainHand and SubHand tag.
	FGameplayTagContainer Filter;
	Filter.AddTag(FCrysGameplayTags::Get().EquipSlot_SubHand);
	EquipSlots = EquipSlots.Filter(Filter);
	EquipSlots.AddTag(FCrysGameplayTags::Get().EquipSlot_MainHand);
}
