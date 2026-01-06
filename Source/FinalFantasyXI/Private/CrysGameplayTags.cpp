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
	GameplayTags.HeroClass = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("HeroClass"), FString("Root Gameplay Tag for Hero classes."));
	GameplayTags.Input = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input"), FString("Root Gameplay Tag for input related events."));
	GameplayTags.Item = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Item"), FString("Root Gameplay Tag for items."));
	GameplayTags.Message = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message"), FString("Root Gameplay Tag to send messages via Gameplay Message Subsystem."));
	GameplayTags.MessageVerb = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("MessageVerb"), FString("Root Gameplay Tag associated with messages via Gameplay Message Subsystem."));
	GameplayTags.WeaponType = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("WeaponType"), FString("Root GameplayTag for weapon types."));

	/**
	 * Attribute Gameplay Tags
	 */
	GameplayTags.Attribute_Level = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Level"), FString("The level of the character."));

	GameplayTags.Attribute_Attack_Melee  = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Attack.Melee"), FString("Melee Attack power."));
	GameplayTags.Attribute_Attack_Range = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Attack.Range"), FString("Range Attack Power."));
	GameplayTags.Attribute_Attack_Technique = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Attack.Technique"), FString("Technique Attack Power."));

	GameplayTags.Attribute_Defense_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Defense.Melee"), FString("Melee defense power."));
	GameplayTags.Attribute_Defense_Range = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Defense.Range"), FString("Range defense power."));
	GameplayTags.Attribute_Defense_Technique = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Defense.Technique"), FString("Technique defense power."));

	GameplayTags.Attribute_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Dexterity"), FString("Dexterity increases the minimum damage threshold."));
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
