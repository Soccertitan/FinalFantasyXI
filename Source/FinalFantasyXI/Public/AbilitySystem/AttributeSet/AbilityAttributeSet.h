// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "AbilityAttributeSet.generated.h"

/**
 * A non replicated set that is captured from source when applying gameplay effects. Like damage or buffs.
 */
UCLASS()
class FINALFANTASYXI_API UAbilityAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UAbilityAttributeSet();
	
	ATTRIBUTE_ACCESSORS(ThisClass, OutgoingPotency);
	ATTRIBUTE_ACCESSORS(ThisClass, OutgoingProbability);
	ATTRIBUTE_ACCESSORS(ThisClass, OutgoingCriticalHitChance);
	ATTRIBUTE_ACCESSORS(ThisClass, OutgoingDefensePierce);

	ATTRIBUTE_ACCESSORS(ThisClass, AbilityCooldownMultiplier);
	ATTRIBUTE_ACCESSORS(ThisClass, AbilityCostMultiplier);
	ATTRIBUTE_ACCESSORS(ThisClass, EnmityMultiplier);
	
protected:
	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
private:
	/** Potency. A generic attribute for boosting the effectiveness of abilities. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData OutgoingPotency;
	
	/** Base likelihood of an ability hitting or an effect happening. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData OutgoingProbability;
	
	/** The base chance for the ability to be a critically effective. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData OutgoingCriticalHitChance;
	
	/** Ignores a percentage of the defenders defense. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData OutgoingDefensePierce;

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
