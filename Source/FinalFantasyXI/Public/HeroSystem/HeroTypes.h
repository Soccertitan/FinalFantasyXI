// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "HeroTypes.generated.h"

class UHeroRaceDefinition;
class UHeroClassDefinition;
class UHeroClassProgressComponent;

/**
 * Contains data about the progress of an item.
 */
USTRUCT(BlueprintType)
struct FHeroClassProgressItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	// The identifier for this progress item.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "HeroClass"))
	FGameplayTag HeroClassTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 1))
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 Experience = 0;

	//~ Begin of FFastArraySerializerItem
	void PostReplicatedAdd(const struct FHeroClassProgressContainer& InSerializer);
	void PostReplicatedChange(const FHeroClassProgressContainer& InSerializer);
	//~ End of FFastArraySerializerItem

	bool IsValid() const;

	FORCEINLINE bool operator==(const FGameplayTag& Other) const
	{
		return HeroClassTag == Other;
	}
	FORCEINLINE bool operator!=(const FGameplayTag& Other) const
	{
		return HeroClassTag != Other;
	}

private:

	friend struct FHeroClassProgressContainer;
	friend class UHeroManagerComponent;
};

USTRUCT(BlueprintType)
struct FHeroClassProgressContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	FHeroClassProgressContainer() {}

	void AddHeroClassProgressItem(const FHeroClassProgressItem& NewItem);

	/**
	 * Adds the specified amount of experience to the item with the HeroClassTag. If it goes beyond the required exp for
	 * the level, it will add levels to the item.
	 */
	void AddExperience(const FGameplayTag& HeroClassTag, const FScalableFloat& ExperienceRequirement, int32 Experience);

	FHeroClassProgressItem GetHeroClassProgressItem(const FGameplayTag& HeroClassTag) const;

	/** Removes all progress in the Container. */
	void Reset();

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FastArrayDeltaSerialize<FHeroClassProgressItem, FHeroClassProgressContainer>(Items, DeltaParms, *this);
	}
	
private:
	UPROPERTY(EditAnywhere)
	TArray<FHeroClassProgressItem> Items;

	UPROPERTY(NotReplicated)
	TObjectPtr<UHeroManagerComponent> Owner;

	friend struct FHeroClassProgressItem;
	friend UHeroManagerComponent;
};
template<>
struct TStructOpsTypeTraits<FHeroClassProgressContainer> : TStructOpsTypeTraitsBase2<FHeroClassProgressContainer>
{
	enum { WithNetDeltaSerializer = true };
};

/** A struct with information on each of the base attributes growth rate. */
USTRUCT()
struct FHeroBaseAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FScalableFloat MaxHitPoints;

	UPROPERTY(EditAnywhere)
	FScalableFloat MaxTacticalPoints;

	UPROPERTY(EditAnywhere)
	FScalableFloat MeleeAttack;

	UPROPERTY(EditAnywhere)
	FScalableFloat MeleeDefense;

	UPROPERTY(EditAnywhere)
	FScalableFloat RangeAttack;

	UPROPERTY(EditAnywhere)
	FScalableFloat RangeDefense;

	UPROPERTY(EditAnywhere)
	FScalableFloat TechniqueAttack;

	UPROPERTY(EditAnywhere)
	FScalableFloat TechniqueDefense;

	UPROPERTY(EditAnywhere)
	FScalableFloat Dexterity;
};

/** Struct used by the HeroManager to quickly get the BaseAttribute values with a given HeroClass, Race, and Level.*/
USTRUCT(BlueprintType)
struct FHeroBaseAttributesCalculated
{
	GENERATED_BODY()

	FHeroBaseAttributesCalculated() {}
	FHeroBaseAttributesCalculated(const UHeroClassDefinition* HeroClass, const UHeroRaceDefinition* HeroRace, int32 Level);

	UPROPERTY(BlueprintReadOnly)
	int32 MaxHitPoints = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 MaxTacticalPoints = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 MeleeAttack = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 MeleeDefense = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 RangeAttack = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 RangeDefense = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 TechniqueAttack = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 TechniqueDefense = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 Dexterity = 1;
};
