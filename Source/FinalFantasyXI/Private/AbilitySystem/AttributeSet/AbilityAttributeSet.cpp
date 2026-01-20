// Copyright Soccertitan 2025


#include "AbilitySystem/AttributeSet/AbilityAttributeSet.h"

UAbilityAttributeSet::UAbilityAttributeSet()
{
	InitAbilityCooldownMultiplier(1.f);
	InitAbilityCostMultiplier(1.f);
	InitEnmityMultiplier(1.f);
}

void UAbilityAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	NewValue = FMath::Max(NewValue, 0.f);
}
