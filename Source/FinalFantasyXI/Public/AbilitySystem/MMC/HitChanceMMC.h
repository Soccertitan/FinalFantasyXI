// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrysGameplayMMC.h"
#include "HitChanceMMC.generated.h"

/**
 * Designed to modify hit chance based on the Accuracy/Evasion stats of Attacker/Defender and level.
 */
UCLASS()
class FINALFANTASYXI_API UHitChanceMMC : public UCrysGameplayMMC
{
	GENERATED_BODY()
	
public:
	UHitChanceMMC();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	virtual const TArray<FGameplayEffectAttributeCaptureDefinition>& GetAttributeCaptureDefinitions() const override;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif WITH_EDITOR
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition AttackerLevelAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition DefenderLevelAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition AccuracyAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition EvasionAttributeDef;
	
	/** Modifies the accuracy of the attacker based on the level difference. (AttackerLevel - DefenderLevel) * LevelModifierAccuracy */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation")
	FScalableFloat LevelModifierAccuracy = 4.f;
	/** (Accuracy - Evasion) * AccuracyEvasionHitChance */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation")
	FScalableFloat AccuracyEvasionHitChance = 0.005f;
	/** If the source has any of these tags, ignore the evasion attribute on the target. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation")
	FGameplayTagContainer IgnoreEvasionAttributeTagContainer;
	
	//** Combines the Relevant Attributes to Capture with the other attributes defined in this class. */
	UPROPERTY()
	TArray<FGameplayEffectAttributeCaptureDefinition> AggregatedRelevantAttributesToCapture;
	
	/** Called in the constructor and PostEditChangeProperty. */
	virtual void UpdateAggregatedRelevantAttributesToCapture();
};
