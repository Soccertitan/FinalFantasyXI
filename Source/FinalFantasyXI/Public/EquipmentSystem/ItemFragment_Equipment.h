// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "EquipmentTypes.h"
#include "AbilitySystem/ItemFragment_Ability.h"
#include "AbilitySystem/Ability/Combat/CombatTypes.h"

#include "ItemFragment_Equipment.generated.h"

class UCombatAnimationData;
class UEquipmentManagerComponent;
class UAbilitySet;

//-----------------------------------------------------------------------------------------------------------
// ItemShard and ItemFragment for a base Equipment.
//-----------------------------------------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct FItemShard_Equipment : public FItemShard
{
	GENERATED_BODY()

	/** The current level of the equipment. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 Level = 0;

	/** The EquipmentManagerComponent the item is currently equipped to. */
	UEquipmentManagerComponent* GetEquipmentManagerComponent() const;

	virtual bool IsMatching(const TInstancedStruct<FItemShard>& Shard) const override;

private:

	/** The EquipmentManagerComponent the item is currently equipped to. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;

	friend UEquipmentManagerComponent;
};

/** The basic ItemFragment that any equipment type can derive from. */
USTRUCT(BlueprintType)
struct FItemFragment_Equipment : public FItemFragment
{
	GENERATED_BODY()

	FItemFragment_Equipment();

	/** The max level an equipment can be upgraded to. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (ClampMin = 0))
	int32 MaxLevel = 0;

	/** The slots the equipment is allowed to be equipped in. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (Categories = "EquipSlot"))
	FGameplayTagContainer EquipSlots;

	/** Unequips items in these slots when equipped and prevents other items from being equipped in the slots. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (Categories = "EquipSlot"))
	FGameplayTagContainer BlockEquipSlots;

	/** A level Requirement of 0 means no requirement. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (ClampMin = 0))
	int32 LevelRequirement = 0;

	/** The character must be one of the specified jobs to equip the item. If empty, all jobs are allowed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (Categories = "HeroJob"))
	FGameplayTagContainer HeroJobs;

	/** Gameplay effect to grant applied at the item level. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	TSubclassOf<UGameplayEffect> GameplayEffect;

	/** The Equipment ItemShard type to create. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment", AdvancedDisplay)
	TInstancedStruct<FItemShard_Equipment> EquipmentShard;

	virtual void SetDefaultValues(TInstancedStruct<FItem>& Item) const override;
	
	virtual void PostSerialize(const FArchive& Ar);
};
template<>
struct TStructOpsTypeTraits<FItemFragment_Equipment> : public TStructOpsTypeTraitsBase2<FItemFragment_Equipment>
{
	enum
	{
		WithPostSerialize = true,
   };
};

//-----------------------------------------------------------------------------------------------------------
// ItemFragment for Weapons
//-----------------------------------------------------------------------------------------------------------

/** An equipment ItemFragment that is a weapon. */
USTRUCT(BlueprintType)
struct FItemFragment_Weapon : public FItemFragment_Equipment
{
	GENERATED_BODY()

	FItemFragment_Weapon(){}

	/** The skill the weapon uses to determine effectiveness. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Weapon", meta = (Categories = "WeaponSkill"))
	FGameplayTag WeaponSkill;

	/** Base damage of the weapon. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Weapon")
	FScalableFloat Damage;

	/** The type of damage this weapon applies. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Weapon", meta = (Categories = "DamageType"))
	FGameplayTag DamageType;

	/** The effect class to use during an auto attack. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Weapon")
	TSoftClassPtr<UGameplayEffect> AutoAttackGameplayEffectClass;

	/** The auto attack delay in seconds between attacks. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Weapon")
	FScalableFloat Delay;

	/** The auto attack range. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Weapon")
	FScalableFloat Range;

	/** Maps to CombatAnimationData on the character. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Weapon")
	FGameplayTag PrimaryCombatAnimationDataTag;

	/** Maps to CombatAnimationData on the character for dual wielding. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Weapon")
	FGameplayTag SecondaryCombatAnimationDataTag;
	
	virtual void PostSerialize(const FArchive& Ar) override;
};
template<>
struct TStructOpsTypeTraits<FItemFragment_Weapon> : public TStructOpsTypeTraitsBase2<FItemFragment_Weapon>
{
	enum
	{
		WithPostSerialize = true,
   };
};
