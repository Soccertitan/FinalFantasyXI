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
	FGameplayEffectAttributeCaptureDefinition HitChanceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition CriticalHitChanceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition DefensePierceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition BaseCumulativeEnmityAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition BaseVolatileEnmityAttributeDef;

	/** The attribute the damage will be applied to on the target. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition IncomingDamageAttributeDef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Attacker")
	FGameplayEffectAttributeCaptureDefinition AttackAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Attacker")
	FGameplayEffectAttributeCaptureDefinition AttackDefenseRatioCapAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Attacker")
	FGameplayEffectAttributeCaptureDefinition CriticalHitBonusAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Attacker")
	FGameplayEffectAttributeCaptureDefinition DamageMultiplierAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Attacker")
	FGameplayEffectAttributeCaptureDefinition CombatSkillAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Attacker")
	FGameplayEffectAttributeCaptureDefinition EnmityMultiplierAttributeDef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender")
	FGameplayEffectAttributeCaptureDefinition DefenseAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender")
	FGameplayEffectAttributeCaptureDefinition ResistanceAttributeDef;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender|Guard")
	FGameplayEffectAttributeCaptureDefinition GuardChanceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender|Guard")
	FGameplayEffectAttributeCaptureDefinition GuardHalfAngleAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender|Guard")
	FGameplayEffectAttributeCaptureDefinition GuardDamageReductionAttributeDef;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender|Parry")
	FGameplayEffectAttributeCaptureDefinition ParryChanceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender|Parry")
	FGameplayEffectAttributeCaptureDefinition ParryHalfAngleAttributeDef;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender|Shield")
	FGameplayEffectAttributeCaptureDefinition BlockChanceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender|Shield")
	FGameplayEffectAttributeCaptureDefinition BlockHalfAngleAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Defender|Shield")
	FGameplayEffectAttributeCaptureDefinition BlockDamageReductionAttributeDef;
	
	/** Rolls a random number between min and max. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation")
	FScalableFloat DamageRandomizerMin = 1.f;
	/** Rolls a random number between min and max. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation")
	FScalableFloat DamageRandomizerMax = 1.05f;
	
	/** Clamps the calculated HitChance between Min and Max HitChance. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|HitChance")
	FScalableFloat MinHitChance = 0.f;
	/** Clamps the calculated HitChance between Min and Max HitChance. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|HitChance")
	FScalableFloat MaxHitChance = 1.f;
	/** If the source has any of these tags, guarantees a hit. Has priority over PerfectEvasion. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|HitChance")
	FGameplayTagContainer PerfectHitTagContainer;
	/** If the target has any of these tags, prevents a hit. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|HitChance")
	FGameplayTagContainer PerfectEvasionTagContainer;
	
	/** Clamps the calculated CriticalHitChance between Min and Max CriticalHitChance. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|CriticalHitChance")
	FScalableFloat MinCriticalHitChance = 0.f;
	/** Clamps the calculated CriticalHitChance between Min and Max CriticalHitChance. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|CriticalHitChance")
	FScalableFloat MaxCriticalHitChance = 1.f;
	/** If the source has any of these tags, guarantees a critical hit. Has priority over ImmuneCriticalHit. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|CriticalHitChance")
	FGameplayTagContainer PerfectCriticalHitTagContainer;
	/** If the target has any of these tags, prevents a critical hit. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|CriticalHitChance")
	FGameplayTagContainer ImmuneCriticalHitTagContainer;
	
	/** Clamps the calculated Guard chance between Min and Max. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|Guard")
	FScalableFloat MinGuardChance = 0.f;
	/** Clamps the calculated Guard chance between Min and Max. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|Guard")
	FScalableFloat MaxGuardChance = 1.f;
	
	/** Clamps the calculated Parry chance between Min and Max. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|Parry")
	FScalableFloat MinParryChance = 0.f;
	/** Clamps the calculated Parry chance between Min and Max. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|Parry")
	FScalableFloat MaxParryChance = 1.f;
	
	/** Clamps the calculated Block chance between Min and Max. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|Block")
	FScalableFloat MinBlockChance = 0.f;
	/** Clamps the calculated Block chance between Min and Max. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|Block")
	FScalableFloat MaxBlockChance = 1.f;

	//** Combines the Relevant Attributes to Capture with the other attributes defined in this class. */
	UPROPERTY()
	TArray<FGameplayEffectAttributeCaptureDefinition> AggregatedRelevantAttributesToCapture;
	
	/** Called in the constructor and PostEditChangeProperty. */
	virtual void UpdateAggregatedRelevantAttributesToCapture();
	
	virtual float CalculateDamage(const float BaseDamage, const bool bCriticalHit, const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const;
	
	bool IsHit(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const;
	bool IsCriticalHit(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const;
	bool IsParried(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const;
	bool IsGuarded(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const;
	bool IsBlocked(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const;
};
