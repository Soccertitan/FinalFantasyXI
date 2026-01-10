// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "GuardAttributeSet.generated.h"

/**
 * Attributes related to guarding an attack *Different from blocking with a shield.
 */
UCLASS()
class FINALFANTASYXI_API UGuardAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UGuardAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(ThisClass, GuardSkill);
	ATTRIBUTE_ACCESSORS(ThisClass, GuardChance);
	ATTRIBUTE_ACCESSORS(ThisClass, GuardAngle);
	ATTRIBUTE_ACCESSORS(ThisClass, GuardDamageReduction);
	
protected:
	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	UFUNCTION()
	void OnRep_GuardSkill(const FGameplayAttributeData& OldValue);
	
private:
	
	/** Boost the likelihood of guarding. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_GuardSkill, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData GuardSkill;
	
	/** The base chance of guarding. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData GuardChance;
	
	/** The front angle valid for guarding. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData GuardAngle;
	
	/** The reduction in damage from guarding. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData GuardDamageReduction;
};
