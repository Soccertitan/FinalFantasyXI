// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/CrysGameplayAbility.h"
#include "CombatGameplayAbility.generated.h"

/**
 * An ability that listens for changes to the primary and secondary combat animations.
 */
UCLASS(Abstract)
class FINALFANTASYXI_API UCombatGameplayAbility : public UCrysGameplayAbility
{
	GENERATED_BODY()
};
