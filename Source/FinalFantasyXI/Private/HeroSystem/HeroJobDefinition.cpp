// Copyright Soccertitan 2025


#include "HeroSystem/HeroJobDefinition.h"

#include "UObject/AssetRegistryTagsContext.h"

UHeroJobDefinition::UHeroJobDefinition()
{
	AssetType = TEXT("HeroJobDefinition");
}

FPrimaryAssetId UHeroJobDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(AssetType, GetFName());
}

void UHeroJobDefinition::GetAssetRegistryTags(FAssetRegistryTagsContext Context) const
{
	Super::GetAssetRegistryTags(Context);

	// Adding the JobTag to the AssetRegistry
	FAssetRegistryTag RegistryTag;
	RegistryTag.Type = FAssetRegistryTag::TT_Alphabetical;
	RegistryTag.Name = "JobTag";
	RegistryTag.Value = JobTag.ToString();
	Context.AddTag(RegistryTag);
}
