// Copyright Soccertitan 2025


#include "AbilitySystem/AttributeSet/CombatAttributeSet.h"

#include "Net/UnrealNetwork.h"

UCombatAttributeSet::UCombatAttributeSet()
{
	InitAttack(1.f);
	InitDefense(1.f);
	InitAccuracy(1.f);
	InitEvasion(1.f);

	InitCriticalHitChance(0.05f);
}

void UCombatAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Attack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Defense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Resistance, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitChance, COND_None, REPNOTIFY_Always);
}

void UCombatAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributes(Attribute, NewValue);
	
	if (Attribute == GetAttackAttribute() || Attribute == GetDefenseAttribute())
	{
		// Attack and Defense values get divided into each other. Ensure Attack and Defense are never less than 1.
		NewValue = FMath::Max(NewValue, 1.f);
		return;
	}
	
	if (Attribute == GetCriticalHitChanceAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
}

void UCombatAttributeSet::OnRep_Attack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Attack, OldValue);
}

void UCombatAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Defense, OldValue);
}

void UCombatAttributeSet::OnRep_Resistance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Resistance, OldValue);
}

void UCombatAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalHitChance, OldValue);
}
