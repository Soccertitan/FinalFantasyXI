// Copyright Soccertitan 2025


#include "AbilitySystem/Ability/Combat/CombatStanceGameplayAbility.h"

#include "AbilitySystem/Ability/Combat/CombatAnimationData.h"

UAnimMontage* UCombatStanceGameplayAbility::GetPrimaryEnterCombatAnim() const
{
	if (GetPrimaryCombatAnimationData())
	{
		return GetPrimaryCombatAnimationData()->EnterCombat.Get();
	}
	return nullptr;
}

UAnimMontage* UCombatStanceGameplayAbility::GetPrimaryExitCombatAnim() const
{
	if (GetPrimaryCombatAnimationData())
	{
		return GetPrimaryCombatAnimationData()->ExitCombat.Get();
	}
	return nullptr;
}

UAnimMontage* UCombatStanceGameplayAbility::GetSecondaryEnterCombatAnim() const
{
	if (GetSecondaryCombatAnimationData())
	{
		return GetSecondaryCombatAnimationData()->EnterCombat.Get();
	}
	return nullptr;
}

UAnimMontage* UCombatStanceGameplayAbility::GetSecondaryExitCombatAnim() const
{
	if (GetSecondaryCombatAnimationData())
	{
		return GetSecondaryCombatAnimationData()->ExitCombat.Get();
	}
	return nullptr;
}
