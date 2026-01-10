// Copyright Soccertitan 2025


#include "AbilitySystem/AttributeSet/AbilityAttributeSet.h"

void UAbilityAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	NewValue = FMath::Max(NewValue, 0.f);
}
