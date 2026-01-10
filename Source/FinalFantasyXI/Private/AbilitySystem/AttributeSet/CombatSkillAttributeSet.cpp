// Copyright Soccertitan 2025


#include "AbilitySystem/AttributeSet/CombatSkillAttributeSet.h"

#include "Net/UnrealNetwork.h"

UCombatSkillAttributeSet::UCombatSkillAttributeSet()
{
}

void UCombatSkillAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, WeaponSkill, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MagicSkill, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, EvasionSkill, COND_None, REPNOTIFY_Always);
}

void UCombatSkillAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributes(Attribute, NewValue);
	
	NewValue = FMath::Max(NewValue, 0.0f);
}

void UCombatSkillAttributeSet::OnRep_WeaponSkill(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, WeaponSkill, OldValue);
}

void UCombatSkillAttributeSet::OnRep_MagicSkill(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MagicSkill, OldValue);
}

void UCombatSkillAttributeSet::OnRep_EvasionSkill(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, EvasionSkill, OldValue);
}
