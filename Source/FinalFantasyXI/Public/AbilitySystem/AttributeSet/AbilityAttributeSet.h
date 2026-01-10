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
	ATTRIBUTE_ACCESSORS(ThisClass, OutgoingBasePotency);
	ATTRIBUTE_ACCESSORS(ThisClass, OutgoingBaseAccuracy);
	ATTRIBUTE_ACCESSORS(ThisClass, OutgoingBaseCriticalHitChance);
	ATTRIBUTE_ACCESSORS(ThisClass, OutgoingBaseDefensePierce);
	
protected:
	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
private:
	/** BasePotency. A generic attribute for boosting the effectiveness of abilities. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData OutgoingBasePotency;
	
	/** Base Accuracy of an ability. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData OutgoingBaseAccuracy;
	
	/** The base chance for the ability to be a critically effective. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData OutgoingBaseCriticalHitChance;
	
	/** Ignores a percentage of the defenders defense. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData OutgoingBaseDefensePierce;
};
