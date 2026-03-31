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
	FGameplayTag Input;
	FGameplayTag Item;
	FGameplayTag Job;
	FGameplayTag MagicSkill;
	FGameplayTag Message;
	FGameplayTag MessageVerb;
	FGameplayTag Race;
	FGameplayTag WeaponSkill;
	
	/**
	 * Ability Gameplay Events
	 */
	FGameplayTag Ability_GameplayEvent_AutoAttack;
	FGameplayTag Ability_GameplayEvent_Damage_Mana;
	FGameplayTag Ability_GameplayEvent_Healing_Mana;
	
	/**
	 * Ability State Tags
	 */
	FGameplayTag Ability_State_Block;
	FGameplayTag Ability_State_CombatStance;
	FGameplayTag Ability_State_DualWield;
	FGameplayTag Ability_State_DualWielding;
	FGameplayTag Ability_State_Guard;
	FGameplayTag Ability_State_MovementRooted;
	FGameplayTag Ability_State_Parry;
	FGameplayTag Ability_State_PauseAutoAttack;
	
	/**
	 * Ability State Ignore Tags
	 */
	FGameplayTag Ability_State_Ignore;
	FGameplayTag Ability_State_Ignore_Block;
	FGameplayTag Ability_State_Ignore_Evasion;
	FGameplayTag Ability_State_Ignore_Guard;
	FGameplayTag Ability_State_Ignore_Parry;
	
	/**
	 * Ability State Perfect Tags
	 */
	FGameplayTag Ability_State_Perfect;
	FGameplayTag Ability_State_Perfect_Evasion;
	FGameplayTag Ability_State_Perfect_Hit;
	FGameplayTag Ability_State_Perfect_CriticalHit;
	
	/**
	 * Ability State Immune Tags
	 */
	FGameplayTag Ability_State_Immune_CriticalHit;

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
	FGameplayTag EquipSlot_Hand;
	FGameplayTag EquipSlot_Hand_Main;
	FGameplayTag EquipSlot_Hand_Sub;
	FGameplayTag EquipSlot_Special;
	
	/**
	 * Enhanced Input Modes
	 */
	FGameplayTag EnhancedInput_Modes_UI;

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
