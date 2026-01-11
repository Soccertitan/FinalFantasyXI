// Copyright Soccertitan 2025


#include "AbilitySystem/AttributeSet/AttackerAttributeSet.h"

#include "Net/UnrealNetwork.h"

UAttackerAttributeSet::UAttackerAttributeSet()
{
	InitAttack(1.f);
	InitAccuracy(1.f);

	InitCriticalHitChance(0.05f);
	InitPotencyMultiplier(1.f);
	InitAbilityCooldownMultiplier(1.f);
	InitAbilityCostMultiplier(1.f);
	InitEnmityMultiplier(1.f);
}

void UAttackerAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Attack, COND_None, REPNOTIFY_Always);
}

void UAttackerAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributes(Attribute, NewValue);
	
	if (Attribute == GetAttackAttribute())
	{
		// Attack and Defense values get divided into each other. Ensure Attack and Defense are never less than 1.
		NewValue = FMath::Max(NewValue, 1.f);
		return;
	}
	
	if (Attribute == GetCriticalHitChanceAttribute() || Attribute == GetEnmityMultiplierAttribute() ||
		Attribute == GetPotencyMultiplierAttribute() || Attribute == GetProbabilityAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
}

void UAttackerAttributeSet::OnRep_Attack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Attack, OldValue);
}
