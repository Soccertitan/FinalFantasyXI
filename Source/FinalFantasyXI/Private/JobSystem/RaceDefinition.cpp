// Copyright Soccertitan 2025


#include "JobSystem/RaceDefinition.h"

#include "UObject/AssetRegistryTagsContext.h"

URaceDefinition::URaceDefinition()
{
	AssetType = TEXT("RaceDefinition");
}

FPrimaryAssetId URaceDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(AssetType, GetFName());
}

void URaceDefinition::GetAssetRegistryTags(FAssetRegistryTagsContext Context) const
{
	Super::GetAssetRegistryTags(Context);
}
