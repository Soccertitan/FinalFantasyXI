// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Singleton containing native gameplay tags.
 */

struct FCrysGameplayTags
{
	static const FCrysGameplayTags& Get() {return GameplayTags;}
	static void InitializeNativeGameplayTags();

private:
	static FCrysGameplayTags GameplayTags;

public:

	/**
	 * Generic Root Gameplay Tags
	 */
	FGameplayTag Attribute;
	FGameplayTag EquipSlot;
	FGameplayTag HeroClass;
	FGameplayTag Input;
	FGameplayTag Item;
	FGameplayTag Message;
	FGameplayTag MessageVerb;
	FGameplayTag WeaponType;

	/**
	 * Attributes
	 */
	FGameplayTag Attribute_Level;

	FGameplayTag Attribute_Attack_Melee;
	FGameplayTag Attribute_Attack_Range;
	FGameplayTag Attribute_Attack_Technique;

	FGameplayTag Attribute_Defense_Melee;
	FGameplayTag Attribute_Defense_Range;
	FGameplayTag Attribute_Defense_Technique;

	FGameplayTag Attribute_Dexterity;

	FGameplayTag Attribute_Bonus;

	/**
	 * EquipSlot Tags
	 */
	FGameplayTag EquipSlot_Weapon;

	/**
	 * Gameplay State Tags
	 */
	FGameplayTag Gameplay_State_MovementRooted;

	/**
	 * Item GameplayTags
	 */
	FGameplayTag Item_Type;

	/**
	 * InventoryContainer GameplayTags
	 */
	FGameplayTag ItemContainer_Abilities;

	/**
	 * Message GameplayTags
	 */
	FGameplayTag Message_CharacterNameUpdated;
	FGameplayTag Message_HeroProgressUpdated;

	/**
	 * Verb GameplayTags for messages
	 */
	FGameplayTag MessageVerb_Added;
	FGameplayTag MessageVerb_LevelUp;
	FGameplayTag MessageVerb_Removed;
	FGameplayTag MessageVerb_Updated;
};
