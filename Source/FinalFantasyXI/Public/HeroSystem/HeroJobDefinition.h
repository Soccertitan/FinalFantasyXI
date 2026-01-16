// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "HeroTypes.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "HeroJobDefinition.generated.h"

class UAbilitySet;

/**
 * Information about a job that characters can be.
 */
UCLASS(Const)
class FINALFANTASYXI_API UHeroJobDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UHeroJobDefinition();
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	virtual void GetAssetRegistryTags(FAssetRegistryTagsContext Context) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Primary Asset")
	FPrimaryAssetType AssetType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Job|UI")
	FText JobName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Job|UI")
	FText ShortName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Job|UI", meta = (MultiLine))
	FText Description;

	/** The tag is used as an identifier to find this Job, Equipment requirements, and other lookups. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Job", meta = (Categories = "HeroJob"))
	FGameplayTag JobTag;

	/** The amount of experience required to rank up. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Job")
	FScalableFloat ExperienceRequirement;
	
	/** Multiplier applied to the Race attributes. */
	UPROPERTY(EditDefaultsOnly, Category = "Job")
	FHeroPrimaryAttributes BaseAttributesMultiplier;

	/** AbilitySet to grant when the MainJob. */
	UPROPERTY(EditDefaultsOnly, Category = "Job", meta = (AssetBundles = "Ability"))
	TSoftObjectPtr<UAbilitySet> MainJobAbilitySet;
	
	/** AbilitySet to grant when the SubJob. */
	UPROPERTY(EditDefaultsOnly, Category = "Job", meta = (AssetBundles = "Ability"))
	TSoftObjectPtr<UAbilitySet> SubJobAbilitySet;
};
