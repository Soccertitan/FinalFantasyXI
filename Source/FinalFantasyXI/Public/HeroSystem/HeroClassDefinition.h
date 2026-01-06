// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "HeroTypes.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "HeroClassDefinition.generated.h"

class UAbilitySet;

/**
 * Information about a Class a Hero can become.
 */
UCLASS(Const)
class FINALFANTASYXI_API UHeroClassDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UHeroClassDefinition();
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	virtual void GetAssetRegistryTags(FAssetRegistryTagsContext Context) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Primary Asset")
	FPrimaryAssetType AssetType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HeroClass|UI")
	FText HeroClassName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HeroClass|UI")
	FText ShortName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HeroClass|UI", meta = (MultiLine))
	FText Description;

	/** The tag is used as an identifier to find this HeroClass, Equipment requirements, and other lookups. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HeroClass", meta = (Categories = "HeroClass"))
	FGameplayTag HeroClassTag;

	/** The amount of experience required to level up. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HeroClass|Attribute")
	FScalableFloat ExperienceRequirement;

	/** The Attributes will be applied based on the level of the character. */
	UPROPERTY(EditDefaultsOnly, Category = "HeroClass|Attribute")
	FHeroBaseAttributes BaseAttributes;

	/** AbilitySet to grant unique traits for the class. */
	UPROPERTY(EditDefaultsOnly, Category = "HeroClass", meta = (AssetBundles = "Ability"))
	TSoftObjectPtr<UAbilitySet> Traits;
};
