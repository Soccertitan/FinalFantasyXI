// Copyright Soccertitan 2025


#include "AbilitySystem/AttributeSet/CrysAttributeSet.h"

#include "Net/UnrealNetwork.h"

UCrysAttributeSet::UCrysAttributeSet()
	: Strength(1.f), Vitality(1.f), Dexterity(1.f), 
	Agility(1.f), Intelligence(1.f), Mind(1.f), Charisma(1.f),
	Attack(1.f), Defense(1.f), Accuracy(1.f), Evasion(1.f), 
	CriticalHitRate(0.05f), MoveSpeed(350.f)
{
}

void UCrysAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Level, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Vitality, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Agility, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Mind, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Charisma, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Attack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Defense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Accuracy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Evasion, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Haste, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Resistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Chance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Bonus, COND_None, REPNOTIFY_Always);
}

void UCrysAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	
	ClampAttribute(Attribute, NewValue);
}

void UCrysAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UCrysAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetLevelAttribute() ||
		Attribute == GetStrengthAttribute() ||
		Attribute == GetVitalityAttribute() ||
		Attribute == GetDexterityAttribute() ||
		Attribute == GetAgilityAttribute() ||
		Attribute == GetIntelligenceAttribute() ||
		Attribute == GetMindAttribute() ||
		Attribute == GetCharismaAttribute() ||
		Attribute == GetAttackAttribute() ||
		Attribute == GetDefenseAttribute() ||
		Attribute == GetAccuracyAttribute() ||
		Attribute == GetEvasionAttribute())
	{
		// Ensure none of these attributes can drop below 1. As they will be divided into.
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	else if (Attribute == GetMoveSpeedAttribute() ||
		Attribute == GetCriticalHitRateAttribute() ||
		Attribute == GetCriticalHitDamageAttribute())
	{
		// Ensure none of these attributes can drop below 0.
		NewValue = FMath::Max(NewValue, 0.f);
	}
}

void UCrysAttributeSet::OnRep_Level(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Level, OldValue);
}

void UCrysAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Strength, OldValue);
}

void UCrysAttributeSet::OnRep_Vitality(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Vitality, OldValue);
}

void UCrysAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Dexterity, OldValue);
}

void UCrysAttributeSet::OnRep_Agility(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Agility, OldValue);
}

void UCrysAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Intelligence, OldValue);
}

void UCrysAttributeSet::OnRep_Mind(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Mind, OldValue);
}

void UCrysAttributeSet::OnRep_Charisma(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Charisma, OldValue);
}

void UCrysAttributeSet::OnRep_Attack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Attack, OldValue);
}

void UCrysAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Defense, OldValue);
}

void UCrysAttributeSet::OnRep_Accuracy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Accuracy, OldValue);
}

void UCrysAttributeSet::OnRep_Evasion(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Evasion, OldValue);
}

void UCrysAttributeSet::OnRep_CriticalHitRate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalHitRate, OldValue);
}

void UCrysAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalHitDamage, OldValue);
}

void UCrysAttributeSet::OnRep_Haste(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Haste, OldValue);
}

void UCrysAttributeSet::OnRep_Resistance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Resistance, OldValue);
}

void UCrysAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MoveSpeed, OldValue);
}

void UCrysAttributeSet::OnRep_Chance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Chance, OldValue);
}

void UCrysAttributeSet::OnRep_Bonus(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Bonus, OldValue);
}
