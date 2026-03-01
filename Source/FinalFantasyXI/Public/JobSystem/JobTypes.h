// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "JobTypes.generated.h"

class UJobDefinition;
class URaceDefinition;

/** Used in the JobManagerComponent to store data related to Jobs and overall character level. */
USTRUCT(BlueprintType)
struct FJobManagerData
{
	GENERATED_BODY()
	
	/** The overall level of the character. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 1))
	int32 Level = 1;

	/** The total cumulative amount of experience achieved by the character. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0))
	int32 Experience = 0;
	
	/** The MaxLevel a character can obtain from leveling. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 1))
	int32 MaxLevel = 50;
	
	/** The max level any job can achieve. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 1))
	int32 MaxJobLevel = 20;
	
	/** Allows the character to equip a SubJob. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSubJobUnlocked = false;
	
	/** A multiplier applied that affects the SubJobs attribute multiplier. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0))
	float SubJobEfficiency = 0.2f;
};

/**
 * Contains data about the progress of an item.
 */
USTRUCT(BlueprintType)
struct FJobProgressItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	// The identifier for this progress item.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Job"))
	FGameplayTag JobTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 1))
	int32 Level = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 Experience = 0;

	//~ Begin of FFastArraySerializerItem
	void PostReplicatedAdd(const struct FJobProgressContainer& InSerializer);
	void PostReplicatedChange(const FJobProgressContainer& InSerializer);
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

	friend struct FJobProgressContainer;
	friend class UJobManagerComponent;
};

USTRUCT(BlueprintType)
struct FJobProgressContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	FJobProgressContainer() {}

	void AddJobProgressItem(const FJobProgressItem& NewItem);

	/**
	 * Adds the specified amount of experience to the item with the JobTag. If it goes beyond the required exp for
	 * the level, it will add levels to the item.
	 */
	void AddExperience(const FGameplayTag& JobTag, const FScalableFloat& ExperienceRequirement, int32 Experience);

	FJobProgressItem GetJobProgressItem(const FGameplayTag& JobTag) const;

	/** Removes all progress in the Container. */
	void Reset();

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FastArrayDeltaSerialize<FJobProgressItem, FJobProgressContainer>(Items, DeltaParms, *this);
	}
	
private:
	UPROPERTY(EditAnywhere)
	TArray<FJobProgressItem> Items;

	UPROPERTY(NotReplicated)
	TObjectPtr<UJobManagerComponent> Owner;

	friend struct FJobProgressItem;
	friend UJobManagerComponent;
	
	void Internal_AddExperience(FJobProgressItem& Item, const FScalableFloat& ExperienceRequirement, int32 Experience);
};
template<>
struct TStructOpsTypeTraits<FJobProgressContainer> : TStructOpsTypeTraitsBase2<FJobProgressContainer>
{
	enum { WithNetDeltaSerializer = true };
};

USTRUCT()
struct FPrimaryAttributes
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

/** Struct used by the JobManager to quickly get the BaseAttribute values with the given Job, Race, and Level.*/
USTRUCT(BlueprintType)
struct FPrimaryAttributesCalculated
{
	GENERATED_BODY()

	FPrimaryAttributesCalculated() {}
	FPrimaryAttributesCalculated(const URaceDefinition* RaceDefinition, int32 Level, 
		const UJobDefinition* MainJob, int32 MainJobRank, 
		const UJobDefinition* SubJob, int32 SubJobRank, float SubJobEfficiency);
	
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
