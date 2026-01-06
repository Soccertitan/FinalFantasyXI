// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "EquipmentTypes.h"
#include "AbilitySystem/ItemFragment_Ability.h"

#include "ItemFragment_Equipment.generated.h"

class UEquipmentManagerComponent;
class UAbilitySet;

//-----------------------------------------------------------------------------------------------------------
// ItemShard and ItemFragment for a base Equipment.
//-----------------------------------------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct FItemShard_Equipment : public FItemShard
{
	GENERATED_BODY()

	/** The current grind level of the equipment. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 GrindLevel = 0;

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	int32 MaxLevel = 10;

	/** Attributes to grant that scale with the item's grind level. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (NoResetToDefault, TitleProperty = "AttributeTag"))
	TArray<FEquipmentAttribute> AttributeValues;

	/** The static abilities to grant on equip. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AssetBundles = "Ability"))
	TSoftObjectPtr<UAbilitySet> AbilitySet;

	/** Dynamic tags to grant to the ASC. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	FGameplayTagContainer DynamicTags;

	/** The equip requirement */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	FEquipRequirement EquipRequirement;

	/** Which slots can this be equipped in. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (Categories = "EquipSlot"))
	FGameplayTagContainer AllowedEquipSlots;

	/** The Equipment ItemShard type to create. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment", NoClear)
	TInstancedStruct<FItemShard_Equipment> EquipmentShard;

	virtual void SetDefaultValues(TInstancedStruct<FItem>& Item) const override;
};

//-----------------------------------------------------------------------------------------------------------
// ItemShard and ItemFragment for Weapons
//-----------------------------------------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct FItemShard_Weapon : public FItemShard_Equipment
{
	GENERATED_BODY()

	/** The current element of the weapon. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, meta = (Categories = "DamageType.Element"))
	FGameplayTag ElementDamageType = FGameplayTag::EmptyTag;

	/** The current power of the elemental damage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 ElementPower = 0;

	virtual bool IsMatching(const TInstancedStruct<FItemShard>& Shard) const override;
};

/** An equipment ItemFragment that is a weapon. */
USTRUCT(BlueprintType)
struct FItemFragment_Weapon : public FItemFragment_Equipment
{
	GENERATED_BODY()

	FItemFragment_Weapon();

	/** Determines the abilities that can be used and animations. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Weapon", meta = (Categories = "WeaponType"))
	FGameplayTag WeaponType;

	/** The physical representation of the weapon. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Weapon", meta = (AssetBundles = "World"))
	TSoftObjectPtr<UStaticMesh> WeaponStaticMesh;
};

//-----------------------------------------------------------------------------------------------------------
// ItemFragment for Armor/Units
//-----------------------------------------------------------------------------------------------------------

/** An equipment ItemFragment that is an armor. */
USTRUCT(BlueprintType)
struct FItemFragment_Armor : public FItemFragment_Equipment
{
	GENERATED_BODY()

	/** The physical representation of the armor. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Armor", meta = (AssetBundles = "World"))
	TSoftObjectPtr<UStaticMesh> LeftArmorStaticMesh;

	/** The physical representation of the armor. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Armor", meta = (AssetBundles = "World"))
	TSoftObjectPtr<UStaticMesh> RightArmorStaticMesh;
};
