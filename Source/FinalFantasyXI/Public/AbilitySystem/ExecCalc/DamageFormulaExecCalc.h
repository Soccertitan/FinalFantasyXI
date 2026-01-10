// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DamageFormulaExecCalc.generated.h"

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UDamageFormulaExecCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
