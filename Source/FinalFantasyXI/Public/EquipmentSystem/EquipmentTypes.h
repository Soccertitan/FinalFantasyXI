// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySet.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "AbilitySystem/Ability/Combat/CombatTypes.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "EquipmentTypes.generated.h"


class UEquipmentManagerComponent;

//---------------------------------------------------------------------------------------------------------
// Fast Array for equipped items.
//---------------------------------------------------------------------------------------------------------

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
	
	/** Equip slots the equipped item blocks. */
	UPROPERTY(BlueprintReadOnly)
	FGameplayTagContainer BlockedEquipSlots;
	
	/** Cached weapon data for quick retrieval. */
	UPROPERTY(BlueprintReadOnly)
	FWeaponData WeaponData;

	UPROPERTY(NotReplicated)
	FActiveGameplayEffectHandle GameplayEffectHandle;

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

	/** Returns a pointer to an EquippedItem. */
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
