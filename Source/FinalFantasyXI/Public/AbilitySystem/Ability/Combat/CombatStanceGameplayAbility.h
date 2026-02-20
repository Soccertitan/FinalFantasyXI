// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CombatGameplayAbility.h"
#include "CombatStanceGameplayAbility.generated.h"

class UCombatAnimationData;

/**
 * Has helper functions to quickly get the proper animations.
 */
UCLASS(Abstract)
class FINALFANTASYXI_API UCombatStanceGameplayAbility : public UCombatGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "ToggleCombatState")
	UAnimMontage* GetPrimaryEnterCombatAnim() const;
	UFUNCTION(BlueprintPure, Category = "ToggleCombatState")
	UAnimMontage* GetPrimaryExitCombatAnim() const;
	
	UFUNCTION(BlueprintPure, Category = "ToggleCombatState")
	UAnimMontage* GetSecondaryEnterCombatAnim() const;
	UFUNCTION(BlueprintPure, Category = "ToggleCombatState")
	UAnimMontage* GetSecondaryExitCombatAnim() const;
};
