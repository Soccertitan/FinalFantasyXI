// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "CombatAttributeSet.generated.h"

/**
 * A collection of attributes that are derived from the primary attribute set.
 */
UCLASS()
class FINALFANTASYXI_API UCombatAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UCombatAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(ThisClass, Attack);
	ATTRIBUTE_ACCESSORS(ThisClass, Defense);
	ATTRIBUTE_ACCESSORS(ThisClass, Accuracy);
	ATTRIBUTE_ACCESSORS(ThisClass, Evasion);
	ATTRIBUTE_ACCESSORS(ThisClass, Resistance);
	
	ATTRIBUTE_ACCESSORS(ThisClass, CriticalHitChance);
	
protected:

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Resistance(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue);
	
private:
	
	/** Attack Power boosted by Strength and Intelligence. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Attack, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Attack;
	/** Defense Power boosted by Vitality and Mind. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Defense, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Defense;
	/** Increases chance to land abilities boosted by Dexterity and Charisma. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Accuracy;
	/** Increases chance to avoid damage and status effects with Agility and Charisma. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Evasion;
	/** The higher the resistance, less damage taken at a percentage level. A value greater than 1 will cause healing. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Resistance, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Resistance;
	
	/** Chance to critically hit */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitChance, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CriticalHitChance;
};
