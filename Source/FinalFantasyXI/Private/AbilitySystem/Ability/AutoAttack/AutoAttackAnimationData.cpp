// Copyright Soccertitan 2025


#include "AbilitySystem/Ability/AutoAttack/AutoAttackAnimationData.h"

UAutoAttackAnimationData::UAutoAttackAnimationData()
{
	AssetType = TEXT("AutoAttackAnimationData");
}

FPrimaryAssetId UAutoAttackAnimationData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(AssetType, GetFName());
}
