// Copyright Soccertitan 2025


#include "Animation/AbilityAnimationData.h"

UAbilityAnimationData::UAbilityAnimationData()
{
	AssetType = TEXT("AbilityAnimationData");
}

FPrimaryAssetId UAbilityAnimationData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(AssetType, GetFName());
}

TSoftObjectPtr<UAnimMontage> UAbilityAnimationData::GetCastingMontage(const FGameplayTag& Tag)
{
	if (TSoftObjectPtr<UAnimMontage>* Montage = CastingMap.Find(Tag))
	{
		return *Montage;
	}
	
	if (CastingMap.IsEmpty())
	{
		return nullptr;
	}
	
	return CastingMap.begin()->Value;
}

TSoftObjectPtr<UAnimMontage> UAbilityAnimationData::GetAbilityMontage(const FGameplayTag& Tag)
{
	if (TSoftObjectPtr<UAnimMontage>* Montage = AbilityMap.Find(Tag))
	{
		return *Montage;
	}
	
	if (AbilityMap.IsEmpty())
	{
		return nullptr;
	}
	
	return AbilityMap.begin()->Value;
}
