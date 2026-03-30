// Copyright Soccertitan 2025


#include "AbilitySystem/AttributeSet/AbilityAttributeSet.h"

#include "Net/UnrealNetwork.h"

UAbilityAttributeSet::UAbilityAttributeSet()
{
	InitCastSpeedMultiplier(1.f);
	InitAbilityCooldownMultiplier(1.f);
	InitAbilityCostMultiplier(1.f);
	InitEnmityMultiplier(1.f);
	InitPotencyMultiplier(1.f);
}

void UAbilityAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CastSpeedMultiplier, COND_OwnerOnly, REPNOTIFY_Always);
}

void UAbilityAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	NewValue = FMath::Max(NewValue, 0.f);
}

void UAbilityAttributeSet::OnRep_CastSpeedMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CastSpeedMultiplier, OldValue);
}
