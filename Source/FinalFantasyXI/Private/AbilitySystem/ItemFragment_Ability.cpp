// Copyright Soccertitan 2025


#include "AbilitySystem/ItemFragment_Ability.h"


bool FItemShard_Ability::IsMatching(const TInstancedStruct<FItemShard>& Fragment) const
{
	if (FItemShard::IsMatching(Fragment))
	{
		if (const FItemShard_Ability* Ptr = Fragment.GetPtr<FItemShard_Ability>())
		{
			if (Ptr->Level == Level)
			{
				return true;
			}
		}
	}
	return false;
}

void FItemFragment_Ability::SetDefaultValues(TInstancedStruct<FItem>& Item) const
{
	TInstancedStruct<FItemShard> NewFragment;
	NewFragment.InitializeAsScriptStruct(FItemShard_Ability::StaticStruct());
	AddItemShard(Item, NewFragment);
}
