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
	GameplayTags.EquipSlot = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EquipSlot"), FString("Root Gameplay Tag for equipment slots."));
	GameplayTags.Job = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("HeroClass"), FString("Root Gameplay Tag for Hero classes."));
	GameplayTags.Input = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input"), FString("Root Gameplay Tag for input related events."));
	GameplayTags.Item = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Item"), FString("Root Gameplay Tag for items."));
	GameplayTags.Message = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message"), FString("Root Gameplay Tag to send messages via Gameplay Message Subsystem."));
	GameplayTags.MessageVerb = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("MessageVerb"), FString("Root Gameplay Tag associated with messages via Gameplay Message Subsystem."));
	GameplayTags.WeaponType = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("WeaponType"), FString("Root GameplayTag for weapon types."));

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
	GameplayTags.Attribute_Defense = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Defense"), FString("A generic attribute for defense attributes."));
	GameplayTags.Attribute_Accuracy = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Accuracy"), FString("A generic attribute for accuracy attributes."));
	GameplayTags.Attribute_Evasion = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Evasion"), FString("A generic attribute for evasion attributes."));
	GameplayTags.Attribute_CriticalHitRate = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.CriticalHitRate"), FString("A generic attribute for critical hit rate attributes."));
	GameplayTags.Attribute_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.CriticalHitDamage"), FString("A generic attribute for critical hit damage attributes."));
	
	GameplayTags.Attribute_Haste = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Haste"), FString("A generic attribute for haste attribute bonus."));
	GameplayTags.Attribute_Resistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resistance"), FString("A generic attribute for resisting damage and status effects."));
	
	GameplayTags.Attribute_MoveSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.MoveSpeed"), FString("Character move speed."));
	
	GameplayTags.Attribute_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Chance"), FString("Generic attributes that have a chance to occur. E.g. block, parry, evasion."));
	GameplayTags.Attribute_Bonus = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Bonus"), FString("Random attributes like, increased exp, drop chance, etc."));

	/**
	 * EquipSlot Tags
	 */
	GameplayTags.EquipSlot_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("EquipSlot.Weapon"), FString("Weapon equipment slot."));
	
	/**
	 * Gameplay State Tags
	 */
	GameplayTags.Gameplay_State_MovementRooted = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Gameplay.State.MovementRooted"), FString("The character has the movement rooted status."));

	/**
	 * Item Gameplay Tags
	 */
	GameplayTags.Item_Type = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Item.Type"), FString("Defines the type of an item. I.e Weapon, Unit, Mag, etc."));

	/**
	 * ItemContainer Gameplay Tags
	 */
	GameplayTags.ItemContainer_Abilities = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("ItemContainer.Abilities"), FString("ItemContainer that holds abilities."));

	/**
	 * Item Types
	 */
	
	/**
	 * Message Gameplay Tags
	 */
	GameplayTags.Message_CharacterNameUpdated = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.CharacterNameUpdated"), FString(""));
	GameplayTags.Message_HeroProgressUpdated = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.HeroProgressUpdated"), FString("The hero's progress was updated. Whether that was a level up or exp gained."));

	/**
	 * Verb Gameplay Tags
	 */
	GameplayTags.MessageVerb_Added = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("MessageVerb.Added"), FString("An entry was added to a container."));
	GameplayTags.MessageVerb_LevelUp = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("MessageVerb.LevelUp"), FString("An entry in the container leveled up."));
	GameplayTags.MessageVerb_Removed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("MessageVerb.Removed"), FString("An entry was removed from the container."));
	GameplayTags.MessageVerb_Updated = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("MessageVerb.Updated"), FString("An entry was updated in the container."));
}
