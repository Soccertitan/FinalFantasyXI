// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UExecCalc_Damage();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	virtual const TArray<FGameplayEffectAttributeCaptureDefinition>& GetAttributeCaptureDefinitions() const override;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif WITH_EDITOR
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	FGameplayEffectAttributeCaptureDefinition BaseDamageAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	FGameplayEffectAttributeCaptureDefinition BaseHitChanceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	FGameplayEffectAttributeCaptureDefinition BaseCriticalHitChanceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	FGameplayEffectAttributeCaptureDefinition BaseDefensePierceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	FGameplayEffectAttributeCaptureDefinition BaseCumulativeEnmityAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Attributes)
	FGameplayEffectAttributeCaptureDefinition BaseVolatileEnmityAttributeDef;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition AttackerLevelAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition DefenderLevelAttributeDef;
	
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes|Attacker")
	FGameplayEffectAttributeCaptureDefinition EnmityMultiplierAttributeDef;
	
	/** Clamps the calculated HitChance between Min and Max HitChance.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|HitChance", meta = (ClampMin=0, ClampMax=1))
	FScalableFloat MinHitChance = 0.f;
	/** Clamps the calculated HitChance between Min and Max HitChance.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|HitChance", meta = (ClampMin=0, ClampMax=1))
	FScalableFloat MaxHitChance = 1.f;
	/** Modifies the accuracy of the attacker based on the level difference. (AttackerLevel - DefenderLevel) * LevelModifierAccuracy */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|HitChance")
	FScalableFloat LevelModifierAccuracy = 4.f;
	/** (Accuracy - Evasion) * AccuracyEvasionHitChance */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|HitChance")
	FScalableFloat AccuracyEvasionHitChance = 0.005f;

	//** Combines the Relevant Attributes to Capture with the other attributes defined in this class. */
	UPROPERTY()
	TArray<FGameplayEffectAttributeCaptureDefinition> AggregatedRelevantAttributesToCapture;
	
	/** Called in the constructor and PostEditChangeProperty. */
	virtual void UpdateAggregatedRelevantAttributesToCapture();
	
	virtual float CalculateHitChance(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParameters) const;
};
