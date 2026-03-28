// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DamageExecCalc.generated.h"

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UDamageExecCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UDamageExecCalc();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	virtual const TArray<FGameplayEffectAttributeCaptureDefinition>& GetAttributeCaptureDefinitions() const override;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif WITH_EDITOR
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition BaseDamageAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition BaseHitChanceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition BaseCriticalHitChanceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition BaseDefensePierceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition BaseCumulativeEnmityAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition BaseVolatileEnmityAttributeDef;
	/** The attribute the damage will be applied to on the target. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition IncomingDamageAttributeDef;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Attacker")
	FGameplayEffectAttributeCaptureDefinition AttackerLevelAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Attacker")
	FGameplayEffectAttributeCaptureDefinition AttackAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Attacker")
	FGameplayEffectAttributeCaptureDefinition AccuracyAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Attacker")
	FGameplayEffectAttributeCaptureDefinition CriticalHitChanceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Attacker")
	FGameplayEffectAttributeCaptureDefinition DefensePierceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Attacker")
	FGameplayEffectAttributeCaptureDefinition CombatSkillAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Attacker")
	FGameplayEffectAttributeCaptureDefinition EnmityMultiplierAttributeDef;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender")
	FGameplayEffectAttributeCaptureDefinition DefenderLevelAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender")
	FGameplayEffectAttributeCaptureDefinition DefenseAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender")
	FGameplayEffectAttributeCaptureDefinition EvasionAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender")
	FGameplayEffectAttributeCaptureDefinition ResistanceAttributeDef;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender|Guard")
	FGameplayEffectAttributeCaptureDefinition GuardChanceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender|Guard")
	FGameplayEffectAttributeCaptureDefinition GuardAngleAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender|Guard")
	FGameplayEffectAttributeCaptureDefinition GuardDamageReductionAttributeDef;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender|Parry")
	FGameplayEffectAttributeCaptureDefinition ParryChanceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender|Parry")
	FGameplayEffectAttributeCaptureDefinition ParryAngleAttributeDef;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender|Shield")
	FGameplayEffectAttributeCaptureDefinition BlockChanceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender|Shield")
	FGameplayEffectAttributeCaptureDefinition BlockAngleAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender|Shield")
	FGameplayEffectAttributeCaptureDefinition BlockDamageReductionAttributeDef;
	
	/** Modifies the accuracy of the attacker based on the level difference. (AttackerLevel - DefenderLevel) * LevelModifierAccuracy */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|HitChance")
	FScalableFloat LevelModifierAccuracy = 4.f;
	/** (Accuracy - Evasion) * AccuracyEvasionHitChance */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|HitChance")
	FScalableFloat AccuracyEvasionHitChance = 0.005f;
	/** Clamps the calculated HitChance between Min and Max HitChance. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|HitChance")
	FScalableFloat MinHitChance = 0.f;
	/** Clamps the calculated HitChance between Min and Max HitChance. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|HitChance")
	FScalableFloat MaxHitChance = 1.f;
	/** If the source has any of these tags, sets the hit chance to 1. Has priority over PerfectEvasion. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|HitChance")
	FGameplayTagContainer PerfectHitTagContainer;
	/** If the target has any of these tags, sets the hit chance to -1. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|HitChance")
	FGameplayTagContainer PerfectEvasionTagContainer;
	/** If the source has any of these tags, ignore the evasion attribute on the target. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|HitChance")
	FGameplayTagContainer IgnoreEvasionAttributeTagContainer;
	
	/** Clamps the calculated CriticalHitChance between Min and Max CriticalHitChance. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|CriticalHitChance")
	FScalableFloat MinCriticalHitChance = 0.f;
	/** Clamps the calculated CriticalHitChance between Min and Max CriticalHitChance. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|CriticalHitChance")
	FScalableFloat MaxCriticalHitChance = 1.f;
	/** If the source has any of these tags, sets the CriticalHitChance to 1. Has priority over PerfectCriticalHitAvoidance. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|CriticalHitChance")
	FGameplayTagContainer PerfectCriticalHitTagContainer;
	/** If the target has any of these tags, sets the CriticalHitChance chance to -1. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|CriticalHitChance")
	FGameplayTagContainer PerfectCriticalHitAvoidanceTagContainer;

	//** Combines the Relevant Attributes to Capture with the other attributes defined in this class. */
	UPROPERTY()
	TArray<FGameplayEffectAttributeCaptureDefinition> AggregatedRelevantAttributesToCapture;
	
	/** Called in the constructor and PostEditChangeProperty. */
	virtual void UpdateAggregatedRelevantAttributesToCapture();
	
	virtual float CalculateHitChance(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const;
	virtual float CalculateBaseDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const;
	virtual float CalculateCriticalHitChance(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const;
};
