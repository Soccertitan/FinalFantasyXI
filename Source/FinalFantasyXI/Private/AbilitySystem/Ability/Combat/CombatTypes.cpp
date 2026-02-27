// Copyright Soccertitan 2025


#include "AbilitySystem/Ability/Combat/CombatTypes.h"

#include "GameplayEffect.h"


bool FWeaponData::IsValid() const
{
	if (!AutoAttackGameplayEffectClass)
	{
		return false;
	}
	return true;
}
