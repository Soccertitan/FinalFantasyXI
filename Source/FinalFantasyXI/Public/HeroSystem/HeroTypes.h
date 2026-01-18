// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "HeroTypes.generated.h"

class UHeroJobDefinition;
class UHeroRaceDefinition;

USTRUCT(BlueprintType)
struct FHeroProgress
{
	GENERATED_BODY()
	
	/** The overall level of the Hero. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 1))
	int32 Level = 1;

	/** The total cumulative amount of experience achieved by the Hero. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 Experience = 0;
	
	/** The MaxLevel a character can obtain from leveling. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 1))
	int32 MaxLevel = 50;
	
	/** The max level any job can achieve. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 1))
	int32 MaxJobLevel = 20;
	
	/** Allows the player to equip a SubJob. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSubJobUnlocked = false;
	
	/** A multiplier applied that affects the SubJobs attribute multiplier. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	float SubJobEfficiency = 0.2f;
};

/**
 * Contains data about the progress of an item.
 */
USTRUCT(BlueprintType)
struct FHeroJobProgressItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	// The identifier for this progress item.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "HeroJob"))
	FGameplayTag JobTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 1))
	int32 Level = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 Experience = 0;

	//~ Begin of FFastArraySerializerItem
	void PostReplicatedAdd(const struct FHeroJobProgressContainer& InSerializer);
	void PostReplicatedChange(const FHeroJobProgressContainer& InSerializer);
	//~ End of FFastArraySerializerItem

	bool IsValid() const;

	FORCEINLINE bool operator==(const FGameplayTag& Other) const
	{
		return JobTag == Other;
	}
	FORCEINLINE bool operator!=(const FGameplayTag& Other) const
	{
		return JobTag != Other;
	}

private:

	friend struct FHeroJobProgressContainer;
	friend class UHeroManagerComponent;
};

USTRUCT(BlueprintType)
struct FHeroJobProgressContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	FHeroJobProgressContainer() {}

	void AddHeroJobProgressItem(const FHeroJobProgressItem& NewItem);

	/**
	 * Adds the specified amount of experience to the item with the HeroClassTag. If it goes beyond the required exp for
	 * the level, it will add levels to the item.
	 */
	void AddExperience(const FGameplayTag& JobTag, const FScalableFloat& ExperienceRequirement, int32 Experience);

	FHeroJobProgressItem GetHeroJobProgressItem(const FGameplayTag& JobTag) const;

	/** Removes all progress in the Container. */
	void Reset();

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FastArrayDeltaSerialize<FHeroJobProgressItem, FHeroJobProgressContainer>(Items, DeltaParms, *this);
	}
	
private:
	UPROPERTY(EditAnywhere)
	TArray<FHeroJobProgressItem> Items;

	UPROPERTY(NotReplicated)
	TObjectPtr<UHeroManagerComponent> Owner;

	friend struct FHeroJobProgressItem;
	friend UHeroManagerComponent;
	
	void Internal_AddExperience(FHeroJobProgressItem& Item, const FScalableFloat& ExperienceRequirement, int32 Experience);
};
template<>
struct TStructOpsTypeTraits<FHeroJobProgressContainer> : TStructOpsTypeTraitsBase2<FHeroJobProgressContainer>
{
	enum { WithNetDeltaSerializer = true };
};

USTRUCT()
struct FHeroPrimaryAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FScalableFloat MaxHitPoints;

	UPROPERTY(EditAnywhere)
	FScalableFloat MaxMagicPoints;

	UPROPERTY(EditAnywhere)
	FScalableFloat Strength;

	UPROPERTY(EditAnywhere)
	FScalableFloat Vitality;

	UPROPERTY(EditAnywhere)
	FScalableFloat Dexterity;
	
	UPROPERTY(EditAnywhere)
	FScalableFloat Agility;

	UPROPERTY(EditAnywhere)
	FScalableFloat Intelligence;

	UPROPERTY(EditAnywhere)
	FScalableFloat Mind;

	UPROPERTY(EditAnywhere)
	FScalableFloat Charisma;
};

/** Struct used by the HeroManager to quickly get the BaseAttribute values with a given HeroClass, Race, and Level.*/
USTRUCT(BlueprintType)
struct FHeroPrimaryAttributesCalculated
{
	GENERATED_BODY()

	FHeroPrimaryAttributesCalculated() {}
	FHeroPrimaryAttributesCalculated(const UHeroRaceDefinition* RaceDefinition, int32 Level, 
		const UHeroJobDefinition* MainJob, int32 MainJobRank, 
		const UHeroJobDefinition* SubJob, int32 SubJobRank, float SubJobEfficiency);
	
	int32 CalculateValue(int32 RaceValue, float MainJobMultiplier, float SubJobMultiplier, float SubJobEfficiency);

	UPROPERTY(BlueprintReadOnly)
	int32 MaxHitPoints = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 MaxMagicPoints = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 Strength = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 Vitality = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 Dexterity = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 Agility = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 Intelligence = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 Mind = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 Charisma = 1;
};
