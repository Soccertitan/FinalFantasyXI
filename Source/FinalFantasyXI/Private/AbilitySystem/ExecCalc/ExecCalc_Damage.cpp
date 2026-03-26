// Copyright Soccertitan 2025


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "CrysGameplayTags.h"
#include "AbilitySystem/AttributeSet/AbilityAttributeSet.h"
#include "AbilitySystem/AttributeSet/AttackerAttributeSet.h"
#include "AbilitySystem/AttributeSet/CombatSkillAttributeSet.h"
#include "AbilitySystem/AttributeSet/CrysHitPointsAttributeSet.h"
#include "AbilitySystem/AttributeSet/DefenderAttributeSet.h"
#include "AbilitySystem/AttributeSet/GuardAttributeSet.h"
#include "AbilitySystem/AttributeSet/ParryAttributeSet.h"
#include "AbilitySystem/AttributeSet/PrimaryAttributeSet.h"
#include "AbilitySystem/AttributeSet/ShieldAttributeSet.h"


UExecCalc_Damage::UExecCalc_Damage()
{
	BaseDamageAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	BaseDamageAttributeDef.AttributeToCapture = UAbilityAttributeSet::GetOutgoingPotencyAttribute();
	BaseHitChanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	BaseHitChanceAttributeDef.AttributeToCapture = UAbilityAttributeSet::GetOutgoingProbabilityAttribute();
	BaseCriticalHitChanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	BaseCriticalHitChanceAttributeDef.AttributeToCapture = UAbilityAttributeSet::GetOutgoingCriticalHitChanceAttribute();
	BaseDefensePierceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	BaseDefensePierceAttributeDef.AttributeToCapture = UAbilityAttributeSet::GetOutgoingDefensePierceAttribute();
	BaseCumulativeEnmityAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	BaseCumulativeEnmityAttributeDef.AttributeToCapture = UAbilityAttributeSet::GetOutgoingCumulativeEnmityAttribute();
	BaseVolatileEnmityAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	BaseVolatileEnmityAttributeDef.AttributeToCapture = UAbilityAttributeSet::GetOutgoingVolatileEnmityAttribute();
	
	AttackerLevelAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AttackerLevelAttributeDef.AttributeToCapture = UPrimaryAttributeSet::GetLevelAttribute();
	DefenderLevelAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	DefenderLevelAttributeDef.AttributeToCapture = UPrimaryAttributeSet::GetLevelAttribute();
	
	AttackAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AttackAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetAttackAttribute();
	AccuracyAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AccuracyAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetAccuracyAttribute();
	CriticalHitChanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	CriticalHitChanceAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetCriticalHitChanceAttribute();
	DefensePierceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	DefensePierceAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetDefensePierceAttribute();
	
	CombatSkillAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	CombatSkillAttributeDef.AttributeToCapture = UCombatSkillAttributeSet::GetWeaponSkillAttribute();
	
	DefenseAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	DefenseAttributeDef.AttributeToCapture = UDefenderAttributeSet::GetDefenseAttribute();
	EvasionAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	EvasionAttributeDef.AttributeToCapture = UDefenderAttributeSet::GetEvasionAttribute();
	ResistanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ResistanceAttributeDef.AttributeToCapture = UDefenderAttributeSet::GetResistanceAttribute();
	
	GuardChanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	GuardChanceAttributeDef.AttributeToCapture = UGuardAttributeSet::GetGuardChanceAttribute();
	GuardAngleAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	GuardAngleAttributeDef.AttributeToCapture = UGuardAttributeSet::GetGuardAngleAttribute();
	GuardDamageReductionAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	GuardDamageReductionAttributeDef.AttributeToCapture = UGuardAttributeSet::GetGuardDamageReductionAttribute();
	
	ParryChanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ParryChanceAttributeDef.AttributeToCapture = UParryAttributeSet::GetParryChanceAttribute();
	ParryAngleAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ParryAngleAttributeDef.AttributeToCapture = UParryAttributeSet::GetParryAngleAttribute();
	
	BlockChanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	BlockChanceAttributeDef.AttributeToCapture = UShieldAttributeSet::GetBlockChanceAttribute();
	BlockAngleAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	BlockAngleAttributeDef.AttributeToCapture = UShieldAttributeSet::GetBlockAngleAttribute();
	BlockDamageReductionAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	BlockDamageReductionAttributeDef.AttributeToCapture = UShieldAttributeSet::GetBlockDamageReductionAttribute();

	EnmityMultiplierAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	EnmityMultiplierAttributeDef.AttributeToCapture = UAbilityAttributeSet::GetEnmityMultiplierAttribute();
	
	UpdateAggregatedRelevantAttributesToCapture();
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	FGameplayEffectSpec* Spec = ExecutionParams.GetOwningSpecForPreExecuteMod();
	
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = Spec->CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = Spec->CapturedTargetTags.GetAggregatedTags();
	
	float BaseDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(BaseDamageAttributeDef, EvaluateParameters, BaseDamage);
	
	float Attack = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackAttributeDef, EvaluateParameters, Attack);
	float Defense = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DefenseAttributeDef, EvaluateParameters, Defense);
	
	float HitChance = CalculateHitChance(ExecutionParams, EvaluateParameters);
	float RandomNum = FMath::RandRange(0.f, 1.f);
	bool bHit = HitChance >= RandomNum; 
	
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UHitPointsAttributeSet::GetDamageAttribute(), EGameplayModOp::Override, BaseDamage));
}

const TArray<FGameplayEffectAttributeCaptureDefinition>& UExecCalc_Damage::GetAttributeCaptureDefinitions() const
{
	return AggregatedRelevantAttributesToCapture;
}

#if WITH_EDITOR
void UExecCalc_Damage::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	UpdateAggregatedRelevantAttributesToCapture();
}
#endif

void UExecCalc_Damage::UpdateAggregatedRelevantAttributesToCapture()
{
	AggregatedRelevantAttributesToCapture.Empty();
	AggregatedRelevantAttributesToCapture.Add(BaseDamageAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(BaseHitChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(BaseCriticalHitChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(BaseDefensePierceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(BaseCumulativeEnmityAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(BaseVolatileEnmityAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(AttackerLevelAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(DefenderLevelAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(AttackAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(AccuracyAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(CriticalHitChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(DefensePierceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(CombatSkillAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(DefenseAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(EvasionAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(ResistanceAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(GuardChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(GuardAngleAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(GuardDamageReductionAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(ParryChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(ParryAngleAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(BlockChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(BlockAngleAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(BlockDamageReductionAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(EnmityMultiplierAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Append(RelevantAttributesToCapture);
}

float UExecCalc_Damage::CalculateHitChance(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParameters) const
{
	float HitChance = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(BaseHitChanceAttributeDef, EvaluateParameters, HitChance);
	float AttackerLevel = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackerLevelAttributeDef, EvaluateParameters, AttackerLevel);
	float DefenderLevel = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DefenderLevelAttributeDef, EvaluateParameters, DefenderLevel);
	float Accuracy = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AccuracyAttributeDef, EvaluateParameters, Accuracy);
	float Evasion = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(EvasionAttributeDef, EvaluateParameters, Evasion);
	
	Accuracy = Accuracy + (AttackerLevel - DefenderLevel) * LevelModifierAccuracy.GetValue();
	HitChance = HitChance + (Accuracy - Evasion) * AccuracyEvasionHitChance.GetValue();
	
	return FMath::Clamp(HitChance, MinHitChance.GetValue(), MaxHitChance.GetValue());
}
