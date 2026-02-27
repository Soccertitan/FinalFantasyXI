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
	FGameplayTag CharacterRace;
	FGameplayTag DamageType;
	FGameplayTag EquipSlot;
	FGameplayTag HeroJob;
	FGameplayTag Input;
	FGameplayTag Item;
	FGameplayTag MagicSkill;
	FGameplayTag Message;
	FGameplayTag MessageVerb;
	FGameplayTag WeaponSkill;
	
	/**
	 * Ability Gameplay Events
	 */
	FGameplayTag Ability_GameplayEvent_AutoAttack;

	/**
	 * Attributes
	 */
	FGameplayTag Attribute_Level;

	FGameplayTag Attribute_Strength;
	FGameplayTag Attribute_Vitality;
	FGameplayTag Attribute_Dexterity;
	FGameplayTag Attribute_Agility;
	FGameplayTag Attribute_Intelligence;
	FGameplayTag Attribute_Mind;
	FGameplayTag Attribute_Charisma;
	
	FGameplayTag Attribute_Attack;
	FGameplayTag Attribute_Accuracy;
	FGameplayTag Attribute_CriticalHitChance;
	
	FGameplayTag Attribute_Defense;
	FGameplayTag Attribute_Evasion;
	FGameplayTag Attribute_EvasionSkill;

	FGameplayTag Attribute_MoveSpeedMultiplier;

	/**
	 * EquipSlot Tags
	 */
	FGameplayTag EquipSlot_MainHand;
	FGameplayTag EquipSlot_SubHand;
	FGameplayTag EquipSlot_Special;
	
	/**
	 * Enhanced Input Modes
	 */
	FGameplayTag EnhancedInput_Modes_UI;

	/**
	 * Gameplay State Tags
	 */
	FGameplayTag Gameplay_State_DualWieldAllowed;
	FGameplayTag Gameplay_State_DualWielding;
	FGameplayTag Gameplay_State_MovementRooted;
	FGameplayTag Gameplay_State_PauseAutoAttack;
	FGameplayTag Gameplay_State_CombatStance;

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
};
