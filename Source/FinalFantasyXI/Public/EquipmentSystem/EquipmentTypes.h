// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySet.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "EquipmentTypes.generated.h"


class UEquipmentManagerComponent;
/** Defines the values for an attribute to grant on an equipped item. */
USTRUCT(BlueprintType)
struct FEquipmentAttribute
{
	GENERATED_BODY()

	/** The attribute to grants stats too. Must match the SetByCaller tag in the EquipmentGameplayEffect. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Attribute"))
	FGameplayTag AttributeTag;

	/** This amount of the attribute to grant scaled with the equipment grind level. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FScalableFloat ScalableFloat;
};

/** Defines the attribute requirement to equip an item. */
USTRUCT(BlueprintType)
struct FEquipRequirement
{
	GENERATED_BODY()

	/** The character must be one of the specified classes to equip the item. If empty, all classes are allowed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "HeroClass"))
	FGameplayTagContainer HeroClassRestrictions;

	/** The AttributeTag requirement. If empty, no requirement. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Attribute"))
	FGameplayTag AttributeTag;

	/** The base value the attribute must be to be equipped. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BaseValue = 0;
};

//---------------------------------------------------------------------------------------------------------
// Fast Array for equipped items.
//---------------------------------------------------------------------------------------------------------

/**
 * Contains information on the gameplay effects and AbilitySet granted to an Ability System Component.
 */
USTRUCT()
struct FEquipGrantedHandle
{
	GENERATED_BODY()

	FActiveGameplayEffectHandle GameplayEffectHandle;

	FAbilitySet_GrantedHandles AbilitySet_GrantedHandles;
};

USTRUCT(BlueprintType)
struct FEquippedItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	/** The slot this item is equipped in. */
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag EquipSlot;

	/** The equipped item identifier. */
	UPROPERTY(BlueprintReadOnly)
	FGuid ItemGuid;

	UPROPERTY(NotReplicated)
	FEquipGrantedHandle EquipGrantedHandle;

	//~ Begin of FFastArraySerializerItem
	void PostReplicatedAdd(const struct FEquippedItemsContainer& Container);
	void PreReplicatedRemove(const FEquippedItemsContainer& Container);
	//~ End of FFastArraySerializerItem

	bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FEquippedItemsContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	/** Returns a pointer to an Item. */
	FEquippedItem* FindItemByEquipSlot(const FGameplayTag& EquipSlot) const;
	FEquippedItem* FindItemByItemGuid(const FGuid& ItemGuid) const;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FEquippedItem, FEquippedItemsContainer>(Items, DeltaParams, *this);
	}
	
private:
	UPROPERTY()
	TArray<FEquippedItem> Items;

	UPROPERTY(NotReplicated)
	TObjectPtr<UEquipmentManagerComponent> Owner;
	
	friend UEquipmentManagerComponent;
	friend struct FEquippedItem;
};
template<>
struct TStructOpsTypeTraits<FEquippedItemsContainer> : TStructOpsTypeTraitsBase2<FEquippedItemsContainer>
{
	enum { WithNetDeltaSerializer = true };
};
