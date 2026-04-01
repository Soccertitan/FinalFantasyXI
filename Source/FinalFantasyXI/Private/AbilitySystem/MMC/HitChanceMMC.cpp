// Copyright Soccertitan 2025


#include "AbilitySystem/MMC/HitChanceMMC.h"

#include "CrysNativeGameplayTags.h"
#include "AbilitySystem/AttributeSet/AttackerAttributeSet.h"
#include "AbilitySystem/AttributeSet/DefenderAttributeSet.h"
#include "AbilitySystem/AttributeSet/PrimaryAttributeSet.h"


UHitChanceMMC::UHitChanceMMC()
{
	AttackerLevelAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AttackerLevelAttributeDef.AttributeToCapture = UPrimaryAttributeSet::GetLevelAttribute();
	DefenderLevelAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	DefenderLevelAttributeDef.AttributeToCapture = UPrimaryAttributeSet::GetLevelAttribute();
	
	AccuracyAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AccuracyAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetAccuracyAttribute();
	EvasionAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	EvasionAttributeDef.AttributeToCapture = UDefenderAttributeSet::GetEvasionAttribute();
	
	UpdateAggregatedRelevantAttributesToCapture();
	
	IgnoreEvasionAttributeTagContainer.AddTag(Crys::NativeGameplayTag::Ability_State_Ignore_Evasion);
}

float UHitChanceMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float AttackerLevel = 1.f;
	GetCapturedAttributeMagnitude(AttackerLevelAttributeDef, Spec, EvaluateParams, AttackerLevel);
	float DefenderLevel = 1.f;
	GetCapturedAttributeMagnitude(DefenderLevelAttributeDef, Spec, EvaluateParams, DefenderLevel);
	float Accuracy = 0.f;
	GetCapturedAttributeMagnitude(AccuracyAttributeDef, Spec, EvaluateParams, Accuracy);
	
	float Evasion = 0.f;
	if (IgnoreEvasionAttributeTagContainer.HasAny(*EvaluateParams.SourceTags) == false)
	{
		GetCapturedAttributeMagnitude(EvasionAttributeDef, Spec, EvaluateParams, Evasion);
	}
	
	Accuracy = Accuracy + (AttackerLevel - DefenderLevel) * LevelModifierAccuracy.GetValueAtLevel(Spec.GetLevel());
	
	return (Accuracy - Evasion) * AccuracyEvasionHitChance.GetValueAtLevel(Spec.GetLevel());
}

const TArray<FGameplayEffectAttributeCaptureDefinition>& UHitChanceMMC::GetAttributeCaptureDefinitions() const
{
	return AggregatedRelevantAttributesToCapture;
}

void UHitChanceMMC::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	UpdateAggregatedRelevantAttributesToCapture();
}

void UHitChanceMMC::UpdateAggregatedRelevantAttributesToCapture()
{
	AggregatedRelevantAttributesToCapture.Empty();
	
	AggregatedRelevantAttributesToCapture.Add(AttackerLevelAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(DefenderLevelAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(AccuracyAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(EvasionAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Append(RelevantAttributesToCapture);
}
