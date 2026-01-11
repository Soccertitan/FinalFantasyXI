// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "DefenderAttributeSet.generated.h"

/**
 * Attribute related to mitigating attacks.
 */
UCLASS()
class FINALFANTASYXI_API UDefenderAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UDefenderAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(ThisClass, Defense);
	ATTRIBUTE_ACCESSORS(ThisClass, Evasion);
	ATTRIBUTE_ACCESSORS(ThisClass, EvasionSkill);
	ATTRIBUTE_ACCESSORS(ThisClass, Resistance);
	
protected:
	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_EvasionSkill(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Resistance(const FGameplayAttributeData& OldValue);
	
private:
	
	/** Defense Power boosted by Vitality and Mind. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Defense, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Defense;
	
	/** Increases chance to avoid damage and status effects with Agility and Charisma. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Evasion;
	
	/** Boost the likelihood of evading attacks. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_EvasionSkill, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData EvasionSkill;
	
	/** The higher the resistance, less damage taken at a percentage level. A value greater than 1 will cause healing. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Resistance, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Resistance;
};
