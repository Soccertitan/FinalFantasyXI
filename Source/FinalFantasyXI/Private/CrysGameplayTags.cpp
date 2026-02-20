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
	GameplayTags.HeroJob = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("HeroJob"), FString("Root Gameplay Tag for jobs the hero can be."));
	GameplayTags.Input = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input"), FString("Root Gameplay Tag for input related events."));
	GameplayTags.Item = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Item"), FString("Root Gameplay Tag for items."));
	GameplayTags.Message = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message"), FString("Root Gameplay Tag to send messages via Gameplay Message Subsystem."));
	GameplayTags.MessageVerb = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("MessageVerb"), FString("Root Gameplay Tag associated with messages via Gameplay Message Subsystem."));
	GameplayTags.WeaponType = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("WeaponType"), FString("Root GameplayTag for weapon types."));

	/**
	 * Ability Gameplay Events
	 */
	GameplayTags.Ability_GameplayEvent_AutoAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.GameplayEvent.AutoAttack"), FString("Triggers an auto attack gameplay ability."));
	
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
	GameplayTags.EquipSlot_MainHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EquipSlot.MainHand"), FString("Main Hand slot (typically weapons)."));
	GameplayTags.EquipSlot_SubHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EquipSlot.SubHand"), FString("Sub Hand slot (typically off hand weapons, shields, ammo)."));
	GameplayTags.EquipSlot_Special = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EquipSlot.Special"), FString("Special slot for unique tools (instruments, throwing, etc.)."));
	
	/**
	 * Gameplay State Tags
	 */
	GameplayTags.Gameplay_State_DualWielding = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Gameplay.State.DualWielding"), FString("The character has two one-handed weapons equipped."));
	GameplayTags.Gameplay_State_MovementRooted = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Gameplay.State.MovementRooted"), FString("The character has the movement rooted status."));
	GameplayTags.Gameplay_State_PauseAutoAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Gameplay.State.PauseAutoAttack"), FString("A character with this tag will have the auto attack timer paused. And restarted when removed."));
	GameplayTags.Gameplay_State_CombatStance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Gameplay.State.CombatStance"), FString("A character with this tag are able to perform auto attacks."));

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
