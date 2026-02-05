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
			if (Ptr->GrindLevel == GrindLevel)
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

bool FItemShard_Weapon::IsMatching(const TInstancedStruct<FItemShard>& Shard) const
{
	if (FItemShard_Equipment::IsMatching(Shard))
	{
		if (const FItemShard_Weapon* Ptr = Shard.GetPtr<FItemShard_Weapon>())
		{
			if (Ptr->ElementPower == ElementPower &&
				Ptr->ElementDamageType == ElementDamageType)
			{
				return true;
			}
		}
	}
	return false;
}

FItemFragment_Weapon::FItemFragment_Weapon()
{
	AllowedEquipSlots.AddTag(FCrysGameplayTags::Get().EquipSlot_MainHand);
	EquipmentShard.InitializeAsScriptStruct(FItemShard_Weapon::StaticStruct());
}
