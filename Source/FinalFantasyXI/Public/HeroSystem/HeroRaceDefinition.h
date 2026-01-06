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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RaceDefinition")
	FText HeroRaceName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RaceDefinition", meta = (MultiLine))
	FText Description;

	/** The HeroCharacter to spawn as. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RaceDefinition")
	TSoftClassPtr<AHeroCharacter> HeroCharacter;

	/** The attributes will be applied as the base value scaled with character level. */
	UPROPERTY(EditDefaultsOnly, Category = "RaceDefinition")
	FHeroBaseAttributes BaseAttributes;

	/** AbilitySet to grant unique traits for the Race. */
	UPROPERTY(EditDefaultsOnly, Category = "RaceDefinition")
	TSoftObjectPtr<UAbilitySet> Traits;
};
