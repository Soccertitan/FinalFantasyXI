// Copyright Soccertitan 2025


#include "AbilitySystem/AttributeSet/AttackerAttributeSet.h"

#include "Net/UnrealNetwork.h"

UAttackerAttributeSet::UAttackerAttributeSet()
{
	InitAttack(1.f);
	InitAccuracy(1.f);

	InitCriticalHitChance(0.05f);
	InitAutoAttackDelay(2.f);
	InitCastSpeedMultiplier(1.f);
}

void UAttackerAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Attack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, AutoAttackDelay, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CastSpeedMultiplier, COND_OwnerOnly, REPNOTIFY_Always);
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
	
	if (Attribute == GetCriticalHitChanceAttribute() ||
		Attribute == GetAutoAttackDelayAttribute() ||
		Attribute == GetCastSpeedMultiplierAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
}

void UAttackerAttributeSet::OnRep_Attack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Attack, OldValue);
}

void UAttackerAttributeSet::OnRep_AutoAttackDelay(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, AutoAttackDelay, OldValue);
}

void UAttackerAttributeSet::OnRep_CastSpeedMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CastSpeedMultiplier, OldValue);
}
