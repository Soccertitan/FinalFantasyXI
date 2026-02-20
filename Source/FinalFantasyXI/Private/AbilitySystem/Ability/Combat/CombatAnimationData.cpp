// Copyright Soccertitan 2025


#include "AbilitySystem/Ability/Combat/CombatAnimationData.h"

UCombatAnimationData::UCombatAnimationData()
{
	AssetType = TEXT("CombatAnimationData");
}

FPrimaryAssetId UCombatAnimationData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(AssetType, GetFName());
}
