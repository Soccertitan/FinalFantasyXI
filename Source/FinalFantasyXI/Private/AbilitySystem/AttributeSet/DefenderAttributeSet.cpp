// Copyright Soccertitan 2025


#include "AbilitySystem/AttributeSet/DefenderAttributeSet.h"

#include "Net/UnrealNetwork.h"

UDefenderAttributeSet::UDefenderAttributeSet()
{
	InitDefense(1.f);
	InitEvasion(1.f);
}

void UDefenderAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Defense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, EvasionSkill, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Resistance, COND_None, REPNOTIFY_Always);
}

void UDefenderAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetDefenseAttribute())
	{
		// Attack and Defense values get divided into each other. Ensure Attack and Defense are never less than 1.
		NewValue = FMath::Max(NewValue, 1.f);
		return;
	}
	
	if (Attribute == GetEvasionAttribute() || Attribute == GetEvasionSkillAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
		return;
	}
}

void UDefenderAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Defense, OldValue);
}

void UDefenderAttributeSet::OnRep_EvasionSkill(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, EvasionSkill, OldValue);
}

void UDefenderAttributeSet::OnRep_Resistance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Resistance, OldValue);
}
