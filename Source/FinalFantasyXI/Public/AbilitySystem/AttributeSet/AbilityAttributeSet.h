// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "AbilityAttributeSet.generated.h"

/**
 * A generic attribute set for various attributes related to abilities.
 */
UCLASS()
class FINALFANTASYXI_API UAbilityAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UAbilityAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(ThisClass, OutgoingPotency);
	ATTRIBUTE_ACCESSORS(ThisClass, OutgoingProbability);
	ATTRIBUTE_ACCESSORS(ThisClass, OutgoingCumulativeEnmity);
	ATTRIBUTE_ACCESSORS(ThisClass, OutgoingVolatileEnmity);

	ATTRIBUTE_ACCESSORS(ThisClass, PotencyMultiplier);
	ATTRIBUTE_ACCESSORS(ThisClass, CastSpeedMultiplier);
	ATTRIBUTE_ACCESSORS(ThisClass, AbilityCooldownMultiplier);
	ATTRIBUTE_ACCESSORS(ThisClass, AbilityCostMultiplier);
	ATTRIBUTE_ACCESSORS(ThisClass, EnmityMultiplier);
	
protected:
	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	UFUNCTION()
	void OnRep_CastSpeedMultiplier(const FGameplayAttributeData& OldValue);
	
private:
	/** Potency. A generic attribute for base effectiveness of abilities. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData OutgoingPotency;
	
	/** Base likelihood of an ability hitting or an effect happening. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData OutgoingProbability;
	
	/** The amount of Enmity the ability generates. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData OutgoingCumulativeEnmity;
	
	/** The amount of Enmity the ability generates. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData OutgoingVolatileEnmity;
	
	/** Potency. A generic attribute for multiplying the effectiveness of abilities. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PotencyMultiplier;
	
	/** The multiplier for casting abilities. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CastSpeedMultiplier, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CastSpeedMultiplier;

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
