// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "AttackerAttributeSet.generated.h"

/**
 * Attributes used for offensive and supportive abilities
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

	ATTRIBUTE_ACCESSORS(ThisClass, Probability);
	ATTRIBUTE_ACCESSORS(ThisClass, PotencyMultiplier);

	ATTRIBUTE_ACCESSORS(ThisClass, AbilityCooldownMultiplier);
	ATTRIBUTE_ACCESSORS(ThisClass, AbilityCostMultiplier);

	ATTRIBUTE_ACCESSORS(ThisClass, EnmityMultiplier);
	
protected:

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& OldValue);
	
	
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
	
	//TODO Consider Critical Damage Attribute.
	
	/** A generic attribute for getting the base probability to do something. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Probability;
	/** BasePotency * (PotencyMultiplier). A generic attribute for boosting the effectiveness of abilities. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PotencyMultiplier;
	
	/** Affects the cooldown of abilities. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AbilityCooldownMultiplier;
	/** Affects the cost of abilities. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AbilityCostMultiplier;
	
	/** Adjust the amount of threat generated towards enemies. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData EnmityMultiplier;
};
