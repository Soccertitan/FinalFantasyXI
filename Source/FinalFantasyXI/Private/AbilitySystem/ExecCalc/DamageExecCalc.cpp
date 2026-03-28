// Copyright Soccertitan 2025


#include "AbilitySystem/ExecCalc/DamageExecCalc.h"

#include "CrysGameplayTags.h"
#include "NativeGameplayTags.h"
#include "AbilitySystem/AttributeSet/AbilityAttributeSet.h"
#include "AbilitySystem/AttributeSet/AttackerAttributeSet.h"
#include "AbilitySystem/AttributeSet/CombatSkillAttributeSet.h"
#include "AbilitySystem/AttributeSet/CrysHitPointsAttributeSet.h"
#include "AbilitySystem/AttributeSet/DefenderAttributeSet.h"
#include "AbilitySystem/AttributeSet/GuardAttributeSet.h"
#include "AbilitySystem/AttributeSet/ParryAttributeSet.h"
#include "AbilitySystem/AttributeSet/PrimaryAttributeSet.h"
#include "AbilitySystem/AttributeSet/ShieldAttributeSet.h"

namespace DamageExecCalcTag
{
	UE_DEFINE_GAMEPLAY_TAG_STATIC(Ability_State_Perfect_Evasion, "Ability.State.Perfect.Evasion")
	UE_DEFINE_GAMEPLAY_TAG_STATIC(Ability_State_Perfect_Hit, "Ability.State.Perfect.Hit")
	UE_DEFINE_GAMEPLAY_TAG_STATIC(Ability_State_Ignore_Evasion, "Ability.State.Ignore.Evasion")
	UE_DEFINE_GAMEPLAY_TAG_STATIC(Ability_State_Perfect_CriticalHit, "Ability.State.Perfect.CriticalHit")
	UE_DEFINE_GAMEPLAY_TAG_STATIC(Ability_State_Perfect_CriticalHitAvoidance, "Ability.State.Perfect.CriticalHitAvoidance")
}

UDamageExecCalc::UDamageExecCalc()
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
	
	IncomingDamageAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    IncomingDamageAttributeDef.AttributeToCapture = UHitPointsAttributeSet::GetDamageAttribute();

	AttackerLevelAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AttackerLevelAttributeDef.AttributeToCapture = UPrimaryAttributeSet::GetLevelAttribute();
	AttackAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AttackAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetAttackAttribute();
	CriticalHitChanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	CriticalHitChanceAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetCriticalHitChanceAttribute();
	DefensePierceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	DefensePierceAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetDefensePierceAttribute();
	AccuracyAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AccuracyAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetAccuracyAttribute();
	CombatSkillAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	CombatSkillAttributeDef.AttributeToCapture = UCombatSkillAttributeSet::GetWeaponSkillAttribute();
	EnmityMultiplierAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	EnmityMultiplierAttributeDef.AttributeToCapture = UAbilityAttributeSet::GetEnmityMultiplierAttribute();
	
	DefenderLevelAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	DefenderLevelAttributeDef.AttributeToCapture = UPrimaryAttributeSet::GetLevelAttribute();
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
	
	UpdateAggregatedRelevantAttributesToCapture();
	
	PerfectEvasionTagContainer.AddTag(DamageExecCalcTag::Ability_State_Perfect_Evasion);
	PerfectHitTagContainer.AddTag(DamageExecCalcTag::Ability_State_Perfect_Hit);
	IgnoreEvasionAttributeTagContainer.AddTag(DamageExecCalcTag::Ability_State_Ignore_Evasion);
	
	PerfectCriticalHitTagContainer.AddTag(DamageExecCalcTag::Ability_State_Perfect_CriticalHit);
	PerfectCriticalHitAvoidanceTagContainer.AddTag(DamageExecCalcTag::Ability_State_Perfect_CriticalHitAvoidance);
}

void UDamageExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	FGameplayEffectSpec* Spec = ExecutionParams.GetOwningSpecForPreExecuteMod();
	
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = Spec->CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = Spec->CapturedTargetTags.GetAggregatedTags();
	
	const float HitChance = CalculateHitChance(ExecutionParams, OutExecutionOutput, EvaluateParams);
	const bool bHit = HitChance >= FMath::RandRange(0.f, 1.f);
	
	if (bHit)
	{
		const float BaseDamage = FMath::Floor(CalculateBaseDamage(ExecutionParams, OutExecutionOutput, EvaluateParams));
		const float CriticalHitChance = CalculateCriticalHitChance(ExecutionParams, OutExecutionOutput, EvaluateParams);
		const bool bCriticalHit = CriticalHitChance >= FMath::RandRange(0.f, 1.f);
		
		if (bCriticalHit)
		{
			// Apply CriticalHit
		}
		
		const float FinalDamage = FMath::Floor(BaseDamage);
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(IncomingDamageAttributeDef.AttributeToCapture, EGameplayModOp::Override, FinalDamage));
	}
}

const TArray<FGameplayEffectAttributeCaptureDefinition>& UDamageExecCalc::GetAttributeCaptureDefinitions() const
{
	return AggregatedRelevantAttributesToCapture;
}

#if WITH_EDITOR
void UDamageExecCalc::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	UpdateAggregatedRelevantAttributesToCapture();
}
#endif

void UDamageExecCalc::UpdateAggregatedRelevantAttributesToCapture()
{
	AggregatedRelevantAttributesToCapture.Empty();
	AggregatedRelevantAttributesToCapture.Add(BaseDamageAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(BaseHitChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(BaseCriticalHitChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(BaseDefensePierceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(BaseCumulativeEnmityAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(BaseVolatileEnmityAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(IncomingDamageAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(AttackerLevelAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(AttackAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(AccuracyAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(CriticalHitChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(DefensePierceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(CombatSkillAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(EnmityMultiplierAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(DefenderLevelAttributeDef);
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
	
	AggregatedRelevantAttributesToCapture.Append(RelevantAttributesToCapture);
}

float UDamageExecCalc::CalculateHitChance(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const
{
	if (PerfectHitTagContainer.HasAny(*EvaluateParams.SourceTags))
	{
		return 1.f;
	}
	if (PerfectEvasionTagContainer.HasAny(*EvaluateParams.SourceTags))
	{
		return -1.f;
	}
	
	float HitChance = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(BaseHitChanceAttributeDef, EvaluateParams, HitChance);
	
	float AttackerLevel = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackerLevelAttributeDef, EvaluateParams, AttackerLevel);
	float DefenderLevel = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DefenderLevelAttributeDef, EvaluateParams, DefenderLevel);
	float Accuracy = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AccuracyAttributeDef, EvaluateParams, Accuracy);
	
	float Evasion = 0.f;
	if (IgnoreEvasionAttributeTagContainer.HasAny(*EvaluateParams.SourceTags) == false)
	{
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(EvasionAttributeDef, EvaluateParams, Evasion);
	}
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	Accuracy = Accuracy + (AttackerLevel - DefenderLevel) * LevelModifierAccuracy.GetValueAtLevel(Spec.GetLevel());
	HitChance = HitChance + (Accuracy - Evasion) * AccuracyEvasionHitChance.GetValueAtLevel(Spec.GetLevel());

	return FMath::Clamp(HitChance, MinHitChance.GetValueAtLevel(Spec.GetLevel()), MaxHitChance.GetValueAtLevel(Spec.GetLevel()));
}

float UDamageExecCalc::CalculateBaseDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const
{
	float BaseDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(BaseDamageAttributeDef, EvaluateParams, BaseDamage);
	
	return BaseDamage;
}

float UDamageExecCalc::CalculateCriticalHitChance(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const
{
	if (PerfectCriticalHitTagContainer.HasAny(*EvaluateParams.SourceTags))
	{
		return 1.f;
	}
	if (PerfectCriticalHitAvoidanceTagContainer.HasAny(*EvaluateParams.SourceTags))
	{
		return -1.f;
	}
	
	float CriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(BaseCriticalHitChanceAttributeDef, EvaluateParams, CriticalHitChance);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	return FMath::Clamp(CriticalHitChance, MinHitChance.GetValueAtLevel(Spec.GetLevel()), MaxHitChance.GetValueAtLevel(Spec.GetLevel()));
}
