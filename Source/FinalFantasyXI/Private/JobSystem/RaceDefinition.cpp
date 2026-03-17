// Copyright Soccertitan 2025


#include "JobSystem/RaceDefinition.h"

#include "UObject/AssetRegistryTagsContext.h"


FPrimaryAssetId URaceDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(StaticClass()->GetFName(), GetFName());
}

void URaceDefinition::GetAssetRegistryTags(FAssetRegistryTagsContext Context) const
{
	Super::GetAssetRegistryTags(Context);
}
