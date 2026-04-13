// Copyright Soccertitan 2025


#include "CrysNativeGameplayTags.h"
#include "NativeGameplayTags.h"


namespace Crys::NativeGameplayTag
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Combat_CombatStance, "Ability.Combat.CombatStance", "Triggers an ability to enter/exit combat stance.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_GameplayEvent_AutoAttack, "Ability.GameplayEvent.AutoAttack", "Triggers an auto attack gameplay ability.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_GameplayEvent_Damage_Mana, "Ability.GameplayEvent.Damage.Mana", "Sends a gameplay event when mana is damaged.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_GameplayEvent_Healing_Mana, "Ability.GameplayEvent.Healing.Mana", "Sends a Gameplay event when mana is healed.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_GameplayEvent_Damage_TacticalPoints, "Ability.GameplayEvent.Damage.TacticalPoints", "Sends a Gameplay event when TP is damaged.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_GameplayEvent_Healing_TacticalPoints, "Ability.GameplayEvent.Healing.TacticalPoints", "Sends a Gameplay event when TP is healed.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Block, "Ability.State.Block", "A character can block attacks with this tag.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_CombatStance, "Ability.State.CombatStance", "A character with this tag are able to start the auto attack timer.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_DualWield, "Ability.State.DualWield", "The character may equip two one-handed weapons.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_DualWielding, "Ability.State.DualWielding", "The character has two one-handed weapons equipped.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Guard, "Ability.State.Guard", "A character can guard from attacks with this tag.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_MovementRooted, "Ability.State.MovementRooted", "The character cannot move or rotate.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Parry, "Ability.State.PauseAutoAttack", "A character with this tag will have the auto attack timer paused. And restarted when removed.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_PauseAutoAttack, "Ability.State.Parry", "A character can parry attacks with this tag.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Ignore, "Ability.State.Ignore", "Base tag to ignore characteristics.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Ignore_Block, "Ability.State.Ignore.Block", "Attacks with tag cannot be blocked.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Ignore_Evasion, "Ability.State.Ignore.Evasion", "Attacks with tag cannot be evaded.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Ignore_Guard, "Ability.State.Ignore.Guard", "Attacks with tag are not guardable.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Ignore_Parry, "Ability.State.Ignore.Parry", "Attacks with tag cannot be parried.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Perfect, "Ability.State.Perfect", "Root tag to denote characteristics that will always succeed.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Perfect_Evasion, "Ability.State.Perfect.Evasion", "Guarantees evading an attack/debuff/etc..");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Perfect_Hit, "Ability.State.Perfect.Hit", "Guarantees to land an Gameplay Effect.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Perfect_CriticalHit, "Ability.State.Perfect.CriticalHit", "Guarantees a critical hit.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Immune_CriticalHit, "Ability.State.Immune.CriticalHit", "Prevents from being critically hit.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute, "Attribute", "Root tag for GAS attributes.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Level, "Attribute.Level", "The level of the character.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Strength, "Attribute.Strength", "Boosts physical damage.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Vitality, "Attribute.Vitality", "Increases maximum HP and reduces physical damage.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Dexterity, "Attribute.Dexterity", "Increases the hit and critical hit rate.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Agility, "Attribute.Agility", "Increases evasion and reduces the chance to be hit by a critical strike.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Intelligence, "Attribute.Intelligence", "Increases offensive magic damage.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Mind, "Attribute.Mind", "Increases MP, reduces offensive magic damage, and increases healing effectiveness.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Charisma, "Attribute.Charisma", "Misc boosts to ability effectiveness.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Attack, "Attribute.Attack", "A generic attribute for attack attributes.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Accuracy, "Attribute.Accuracy", "A generic attribute for accuracy attributes.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_CriticalHitChance, "Attribute.CriticalHitChance", "A generic attribute for critical hit chance attributes.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Defense, "Attribute.Defense", "A generic attribute for defense attributes.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Evasion, "Attribute.Evasion", "A generic attribute for evasion attributes.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_EvasionSkill, "Attribute.EvasionSkill", "Enhances the evasion attribute.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_MoveSpeedMultiplier, "Attribute.MoveSpeedMultiplier", "Modifies the base move speed.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CharacterRace, "CharacterRace", "Root GameplayTag for different the different races. Typically for increasing damage against a species of monster.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(DamageType, "DamageType", "Root GameplayTag for different damage types.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EnhancedInput_Modes_UI, "EnhancedInput.Modes.UI", "When a UINav widget is added to the screen. This input mode is added.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EquipSlot, "EquipSlot", "Root GameplayTag for equipment slots.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EquipSlot_Hand, "EquipSlot.Hand", "The equipment can be wielded in both the MainHand and SubHand.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EquipSlot_Hand_Main, "EquipSlot.Hand.Main", "Main Hand slot that can only hold a weapon.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EquipSlot_Hand_Sub, "EquipSlot.Hand.Sub", "Sub Hand slot for dual wielding, shield, ammo, or some other sub hand equipment");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EquipSlot_Special, "EquipSlot.Special", "Special slot for unique tools (instruments, throwing, etc.).");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Damage, "GameplayCue.Damage", "Broadcasts info on a damage effect.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item, "Item", "Root GameplayTag for items.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Type, "Item.Type", "Defines categories of items. (weapon, armor, accessory, spell, etc.)");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ItemContainer_Abilities, "ItemContainer.Abilities", "ItemContainer that holds abilities.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Job, "Job", "Root GameplayTag for jobs characters can be.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(MagicSkill, "MagicSkill", "Root GameplayTag for magic skill types.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Message, "Message", "Root GameplayTag to send messages via Gameplay Message Subsystem.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(MessageVerb, "MessageVerb", "Root GameplayTag associated with messages via Gameplay Message Subsystem.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Message_CharacterNameUpdated, "Message.CharacterNameUpdated", "Broadcasts when the character name updates.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(WeaponSkill, "WeaponSkill", "Root GameplayTag for weapon skill types.");
}
