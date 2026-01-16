// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "HeroTypes.h"
#include "Engine/DataAsset.h"
#include "HeroRaceDefinition.generated.h"


class UAbilitySet;
class AHeroCharacter;

/**
 * Defines the characteristics of a character.
 */
UCLASS(Const)
class FINALFANTASYXI_API UHeroRaceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UHeroRaceDefinition();
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	virtual void GetAssetRegistryTags(FAssetRegistryTagsContext Context) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Primary Asset")
	FPrimaryAssetType AssetType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Race")
	FText HeroRaceName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Race", meta = (MultiLine))
	FText Description;

	/** The HeroCharacter to spawn as. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Race")
	TSoftClassPtr<AHeroCharacter> HeroCharacter;
	
	/** The amount of experience required to level up. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Race")
	FScalableFloat ExperienceRequirement;
	
	/** Base attributes of the race scaled at level. Multiplied by the equipped Main/Sub Jobs. */
	UPROPERTY(EditDefaultsOnly, Category = "Race")
	FHeroPrimaryAttributes BaseAttributes;

	/** AbilitySet to grant. */
	UPROPERTY(EditDefaultsOnly, Category = "Race")
	TSoftObjectPtr<UAbilitySet> Traits;
};
