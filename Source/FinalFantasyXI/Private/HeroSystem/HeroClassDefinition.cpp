// Copyright Soccertitan 2025


#include "HeroSystem/HeroClassDefinition.h"

#include "UObject/AssetRegistryTagsContext.h"

UHeroClassDefinition::UHeroClassDefinition()
{
	AssetType = TEXT("HeroClassDefinition");
}

FPrimaryAssetId UHeroClassDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(AssetType, GetFName());
}

void UHeroClassDefinition::GetAssetRegistryTags(FAssetRegistryTagsContext Context) const
{
	Super::GetAssetRegistryTags(Context);

	// Adding the HeroClassTag to the AssetRegistry
	FAssetRegistryTag RegistryTag;
	RegistryTag.Type = FAssetRegistryTag::TT_Alphabetical;
	RegistryTag.Name = "HeroClassTag";
	RegistryTag.Value = HeroClassTag.ToString();
	Context.AddTag(RegistryTag);
}
