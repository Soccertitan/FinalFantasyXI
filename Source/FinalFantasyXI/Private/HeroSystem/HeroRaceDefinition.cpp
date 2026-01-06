// Copyright Soccertitan 2025


#include "HeroSystem/HeroRaceDefinition.h"

#include "UObject/AssetRegistryTagsContext.h"

UHeroRaceDefinition::UHeroRaceDefinition()
{
	AssetType = TEXT("HeroRaceDefinition");
}

FPrimaryAssetId UHeroRaceDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(AssetType, GetFName());
}

void UHeroRaceDefinition::GetAssetRegistryTags(FAssetRegistryTagsContext Context) const
{
	Super::GetAssetRegistryTags(Context);
}
