// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "CrysGameplayMMC.generated.h"

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UCrysGameplayMMC : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
protected:
	/**
	 * Gets the captured magnitude value for the given Attribute
	 * For this to work correctly, the Attribute needs to be added to the Relevant Attributes to Capture array
	 *
	 * @param EffectSpec The Gameplay Effect Spec to get the info from
	 * @param Definition The attribute capture definition
	 *
	 * @return The magnitude value if found, zero otherwise
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Ability|GameplayEffect", meta=(DisplayName="Get Captured Attribute Magnitude", ScriptName="GetCapturedAttributeMagnitude"))
	float K2_GetCapturedAttributeMagnitudeCrys(const FGameplayEffectSpec& EffectSpec, FGameplayEffectAttributeCaptureDefinition Definition, const FGameplayTagContainer& SourceTags, const FGameplayTagContainer& TargetTags) const;
};
