// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "JobTypes.h"
#include "Engine/DataAsset.h"
#include "RaceDefinition.generated.h"


class UAbilitySet;

/**
 * Defines the characteristics of a character.
 */
UCLASS(Const)
class FINALFANTASYXI_API URaceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	URaceDefinition();
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	virtual void GetAssetRegistryTags(FAssetRegistryTagsContext Context) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Primary Asset")
	FPrimaryAssetType AssetType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Race")
	FText RaceName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Race", meta = (MultiLine))
	FText Description;
	
	/** The tag is used as an identifier to find this Race, Equipment requirements, and other lookups. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Race", meta = (Categories = "Race"))
	FGameplayTag RaceTag;
	
	/** The amount of experience required to level up. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Race")
	FScalableFloat ExperienceRequirement;
	
	/** Base attributes of the race scaled at level. Multiplied by the equipped Main/Sub Jobs. */
	UPROPERTY(EditDefaultsOnly, Category = "Race")
	FPrimaryAttributes BaseAttributes;

	/** AbilitySet to grant. */
	UPROPERTY(EditDefaultsOnly, Category = "Race")
	TSoftObjectPtr<UAbilitySet> Traits;
};
