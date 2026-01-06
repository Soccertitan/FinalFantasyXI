// Copyright Soccertitan 2025


#include "AbilitySystem/CrysGameplayAbilityDefinition.h"

#include "UObject/AssetRegistryTagsContext.h"

UCrysGameplayAbilityDefinition::UCrysGameplayAbilityDefinition()
{
}

void UCrysGameplayAbilityDefinition::GetAssetRegistryTags(FAssetRegistryTagsContext Context) const
{
	Super::GetAssetRegistryTags(Context);

	// Adding AllowedHeroClasses to the AssetRegistry.
	FAssetRegistryTag RegistryTag;
	RegistryTag.Type = FAssetRegistryTag::TT_Alphabetical;
	RegistryTag.Name = "AllowedHeroClasses";
	RegistryTag.Value = AllowedHeroClasses.ToString();
	Context.AddTag(RegistryTag);
}
