// Copyright Soccertitan 2025


#include "CrysGameplayTags.h"

#include "GameplayTagsManager.h"

FCrysGameplayTags FCrysGameplayTags::GameplayTags;

void FCrysGameplayTags::InitializeNativeGameplayTags()
{
	/**
	 * Root Gameplay Tags
	 */
	GameplayTags.Attribute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute"), FString("Root Gameplay Tag for GAS attributes."));
	GameplayTags.CharacterRace = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CharacterRace"), FString("Root Gameplay Tag for different the different races. Typically for increasing damage against a species of monster."));
	GameplayTags.DamageType = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageType"), FString("Root Gameplay Tag for different damage types."));
	GameplayTags.EquipSlot = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EquipSlot"), FString("Root Gameplay Tag for equipment slots."));
	GameplayTags.Input = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input"), FString("Root Gameplay Tag for input related events."));
	GameplayTags.Item = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Item"), FString("Root Gameplay Tag for items."));
	GameplayTags.Job = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Job"), FString("Root Gameplay Tag for jobs characters can be."));
	GameplayTags.MagicSkill = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("MagicSkill"), FString("Root GameplayTag for magic skill types."));
	GameplayTags.Message = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message"), FString("Root Gameplay Tag to send messages via Gameplay Message Subsystem."));
	GameplayTags.MessageVerb = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("MessageVerb"), FString("Root Gameplay Tag associated with messages via Gameplay Message Subsystem."));
	GameplayTags.Race = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Race"), FString("Root Gameplay Tag for races characters can be."));
	GameplayTags.WeaponSkill = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("WeaponSkill"), FString("Root GameplayTag for weapon skill types."));

	/**
	 * Ability Gameplay Events
	 */
	GameplayTags.Ability_GameplayEvent_AutoAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.GameplayEvent.AutoAttack"), FString("Triggers an auto attack gameplay ability."));
	GameplayTags.Ability_GameplayEvent_Damage_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.GameplayEvent.Damage.Mana"), FString("Sends a Gameplay event when mana is damaged."));
	GameplayTags.Ability_GameplayEvent_Healing_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.GameplayEvent.Healing.Mana"), FString("Sends a Gameplay event when mana is healed."));
	
	/**
	 * Gameplay State Tags
	 */
	GameplayTags.Ability_State_Block = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.State.Block"), FString("A character can block attacks with this tag."));
	GameplayTags.Ability_State_DualWield = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.State.DualWield"), FString("The character may equip two one-handed weapons."));
	GameplayTags.Ability_State_DualWielding = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.State.DualWielding"), FString("The character has two one-handed weapons equipped."));
	GameplayTags.Ability_State_Guard = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.State.Guard"), FString("A character can guard from attacks with this tag."));
	GameplayTags.Ability_State_MovementRooted = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.State.MovementRooted"), FString("The character cannot move or rotate."));
	GameplayTags.Ability_State_PauseAutoAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.State.PauseAutoAttack"), FString("A character with this tag will have the auto attack timer paused. And restarted when removed."));
	GameplayTags.Ability_State_Parry = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.State.Parry"), FString("A character can parry attacks with this tag."));
	GameplayTags.Ability_State_CombatStance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.State.CombatStance"), FString("A character with this tag are able to start the auto attack timer."));

	/**
	 * Ability State Ignore Tags
	 */
	GameplayTags.Ability_State_Ignore = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.State.Ignore"), FString("Base tag to ignore characteristics."));
	GameplayTags.Ability_State_Ignore_Block = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.State.Ignore.Block"), FString("Attacks with tag cannot be blocked."));
	GameplayTags.Ability_State_Ignore_Evasion = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.State.Ignore.Evasion"), FString("Attacks with tag cannot be evaded."));
	GameplayTags.Ability_State_Ignore_Guard = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.State.Ignore.Guard"), FString("Attacks with tag are not guardable."));
	GameplayTags.Ability_State_Ignore_Parry = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.State.Ignore.Parry"), FString("Attacks with tag cannot be parried."));
	
	/**
	 * Ability State Perfect Tags
	 */
	GameplayTags.Ability_State_Perfect = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.State.Perfect"), FString("Base tag to denote characteristics that will always succeed."));
	GameplayTags.Ability_State_Perfect_Evasion = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.State.Perfect.Evasion"), FString("Guarantees evading an attack/debuff/etc.."));
	GameplayTags.Ability_State_Perfect_Hit = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.State.Perfect.Hit"), FString("Guarantees to land an Gameplay Effect."));
	GameplayTags.Ability_State_Perfect_CriticalHit = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.State.Perfect.CriticalHit"), FString("Guarantees a critical hit."));
	
	/**
	 * Ability State Immune Tags
	 */
	GameplayTags.Ability_State_Immune_CriticalHit = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.State.Immune.CriticalHit"), FString("Prevents from being critically hit."));
	
	/**
	 * Attribute Gameplay Tags
	 */
	GameplayTags.Attribute_Level = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Level"), FString("The level of the character."));
	
	GameplayTags.Attribute_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Strength"), FString("Strength increases physical damage."));
	GameplayTags.Attribute_Vitality = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Vitality"), FString("Increases maximum HP and reduces physical damage."));
	GameplayTags.Attribute_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Dexterity"), FString("Increases the hit and critical hit rate."));
	GameplayTags.Attribute_Agility = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Agility"), FString("Increases evasion and reduces the chance to be hit by a critical strike."));
	GameplayTags.Attribute_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Intelligence"), FString("Increases offensive magic damage."));
	GameplayTags.Attribute_Mind = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Mind"), FString("Increases MP, reduces offensive magic damage, and increases healing effectiveness."));
	GameplayTags.Attribute_Charisma = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Charisma"), FString("Misc boosts to ability effectiveness."));
	
	GameplayTags.Attribute_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Attack"), FString("A generic attribute for attack attributes."));
	GameplayTags.Attribute_Accuracy = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Accuracy"), FString("A generic attribute for accuracy attributes."));
	GameplayTags.Attribute_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.CriticalHitChance"), FString("A generic attribute for critical hit chance attributes."));
	
	GameplayTags.Attribute_Defense = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Defense"), FString("A generic attribute for defense attributes."));
	GameplayTags.Attribute_Evasion = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Evasion"), FString("A generic attribute for evasion attributes."));
	GameplayTags.Attribute_EvasionSkill = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.EvasionSkill"), FString("Enhances the evasion attribute."));
	
	GameplayTags.Attribute_MoveSpeedMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.MoveSpeed"), FString("Character move speed."));

	/**
	 * Enhanced Input Modes
	 */
	GameplayTags.EnhancedInput_Modes_UI = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EnhancedInput.Modes.UI"), FString("When the UI is added to the screen. This input mode is added."));
	
	/**
	 * EquipSlot Tags
	 */
	GameplayTags.EquipSlot_Hand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EquipSlot.Hand"), FString("Hand slot. An equipment with this can be wielded in both the MainHand and SubHand."));
	GameplayTags.EquipSlot_Hand_Main = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EquipSlot.Hand.Main"), FString("Main Hand slot that can only hold a weapon."));
	GameplayTags.EquipSlot_Hand_Sub = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EquipSlot.Hand.Sub"), FString("Sub Hand slot for dual wielding, shield, ammo, or some other sub hand equipment"));
	GameplayTags.EquipSlot_Special = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EquipSlot.Special"), FString("Special slot for unique tools (instruments, throwing, etc.)."));
	
	/**
	 * Item Gameplay Tags
	 */
	GameplayTags.Item_Type = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Item.Type"), FString("Defines the type of an item. I.e Weapon, Unit, Mag, etc."));

	/**
	 * ItemContainer Gameplay Tags
	 */
	GameplayTags.ItemContainer_Abilities = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("ItemContainer.Abilities"), FString("ItemContainer that holds abilities."));
	
	/**
	 * Message Gameplay Tags
	 */
	GameplayTags.Message_CharacterNameUpdated = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.CharacterNameUpdated"), FString(""));
}
