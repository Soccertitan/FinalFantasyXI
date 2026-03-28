// Copyright Soccertitan 2025


#include "AbilitySystem/MMC/CrysGameplayMMC.h"

float UCrysGameplayMMC::K2_GetCapturedAttributeMagnitudeCrys(const FGameplayEffectSpec& EffectSpec,
	FGameplayEffectAttributeCaptureDefinition Definition, const FGameplayTagContainer& SourceTags,
	const FGameplayTagContainer& TargetTags) const
{
	float Magnitude = 0.0f;
	
	// configure the aggregator evaluation parameters
	// TODO: apply filters?
	FAggregatorEvaluateParameters EvaluationParameters;

	EvaluationParameters.SourceTags = &SourceTags;
	EvaluationParameters.TargetTags = &TargetTags;

	// get the attribute magnitude
	GetCapturedAttributeMagnitude(Definition, EffectSpec, EvaluationParameters, Magnitude);

	return Magnitude;
}
