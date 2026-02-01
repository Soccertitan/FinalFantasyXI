// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "AttackerAttributeSet.generated.h"

/**
 * Attributes used for causing abilities/effects to occur. For example, offensive and supportive abilities.
 */
UCLASS()
class FINALFANTASYXI_API UAttackerAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UAttackerAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(ThisClass, Attack);
	ATTRIBUTE_ACCESSORS(ThisClass, Accuracy);

	ATTRIBUTE_ACCESSORS(ThisClass, DefensePierce);
	ATTRIBUTE_ACCESSORS(ThisClass, CriticalHitChance);
	
	ATTRIBUTE_ACCESSORS(ThisClass, AutoAttackDelay);
	ATTRIBUTE_ACCESSORS(ThisClass, CastSpeedMultiplier);
	
protected:

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_AutoAttackDelay(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_CastSpeedMultiplier(const FGameplayAttributeData& OldValue);
	
private:
	
	/** Attack Power boosted by Strength and Intelligence. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Attack, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Attack;

	/** Increases chance to land abilities boosted by Dexterity and Charisma. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Accuracy;
	
	/** Defense * (1 - DefensePierce). Ignores a percentage of the defenders defense. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DefensePierce;

	/** Chance to critically hit */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CriticalHitChance;
	
	/** The amount of time between auto attacks in seconds. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_AutoAttackDelay, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AutoAttackDelay;
	
	/** The multiplier for casting abilities. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CastSpeedMultiplier, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CastSpeedMultiplier;
};
