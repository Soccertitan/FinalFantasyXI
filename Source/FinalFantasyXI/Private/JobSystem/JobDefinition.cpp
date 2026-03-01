// Copyright Soccertitan 2025


#include "JobSystem/JobDefinition.h"

#include "UObject/AssetRegistryTagsContext.h"

UJobDefinition::UJobDefinition()
{
	AssetType = TEXT("JobDefinition");
}

FPrimaryAssetId UJobDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(AssetType, GetFName());
}

void UJobDefinition::GetAssetRegistryTags(FAssetRegistryTagsContext Context) const
{
	Super::GetAssetRegistryTags(Context);

	// Adding the JobTag to the AssetRegistry
	FAssetRegistryTag RegistryTag;
	RegistryTag.Type = FAssetRegistryTag::TT_Alphabetical;
	RegistryTag.Name = "JobTag";
	RegistryTag.Value = JobTag.ToString();
	Context.AddTag(RegistryTag);
}
