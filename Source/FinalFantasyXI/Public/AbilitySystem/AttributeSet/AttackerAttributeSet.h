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
};
