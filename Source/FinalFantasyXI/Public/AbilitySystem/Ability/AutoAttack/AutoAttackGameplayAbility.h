// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/CrysGameplayAbility.h"
#include "AutoAttackGameplayAbility.generated.h"

/**
 * Listens for an event from the AutoAttackManagerComponent to trigger the auto attack.
 */
UCLASS()
class FINALFANTASYXI_API UAutoAttackGameplayAbility : public UCrysGameplayAbility
{
	GENERATED_BODY()
};
