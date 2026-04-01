// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilityTypes.generated.h"

/**
 * Added to the GameplayEffectContext during the DamageExecCalc.
 */
USTRUCT(BlueprintType)
struct FDamageGameplayContext
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	bool bHit = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool bCriticalHit = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool bGuarded = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool bParried = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool bBlocked = false;
};
