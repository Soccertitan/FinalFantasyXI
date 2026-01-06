// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "CrysAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UCrysAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()

public:
	UCrysAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	ATTRIBUTE_ACCESSORS(ThisClass, Level);
	
	ATTRIBUTE_ACCESSORS(ThisClass, Strength);
	ATTRIBUTE_ACCESSORS(ThisClass, Vitality);
	ATTRIBUTE_ACCESSORS(ThisClass, Dexterity);
	ATTRIBUTE_ACCESSORS(ThisClass, Agility);
	ATTRIBUTE_ACCESSORS(ThisClass, Intelligence);
	ATTRIBUTE_ACCESSORS(ThisClass, Mind);
	ATTRIBUTE_ACCESSORS(ThisClass, Charisma);
	
	ATTRIBUTE_ACCESSORS(ThisClass, Attack);
	ATTRIBUTE_ACCESSORS(ThisClass, Defense);
	ATTRIBUTE_ACCESSORS(ThisClass, Accuracy);
	ATTRIBUTE_ACCESSORS(ThisClass, Evasion);
	ATTRIBUTE_ACCESSORS(ThisClass, CriticalHitRate);
	ATTRIBUTE_ACCESSORS(ThisClass, CriticalHitDamage);

	ATTRIBUTE_ACCESSORS(ThisClass, Haste);
	
	ATTRIBUTE_ACCESSORS(ThisClass, Resistance);
	ATTRIBUTE_ACCESSORS(ThisClass, DamageTaken);

	ATTRIBUTE_ACCESSORS(ThisClass, MoveSpeed);

	ATTRIBUTE_ACCESSORS(ThisClass, Chance);
	ATTRIBUTE_ACCESSORS(ThisClass, Bonus);

protected:
	
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

	UFUNCTION()
	void OnRep_Level(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Vitality(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Agility(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Mind(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Charisma(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Accuracy(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Evasion(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_CriticalHitRate(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Haste(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_Resistance(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_DamageTaken(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MoveSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Chance(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Bonus(const FGameplayAttributeData& OldValue);

private:
	/** The level of the character */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Level, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Level;

	/** Primary Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Strength, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Strength;
	/** Primary Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Vitality, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Vitality;
	/** Primary Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Dexterity, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Dexterity;
	/** Primary Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Agility, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Agility;
	/** Primary Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Intelligence, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Intelligence;
	/** Primary Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mind, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Mind;
	/** Primary Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Charisma, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Charisma;
	
	/** Secondary Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Attack, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Attack;
	/** Secondary Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Defense, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Defense;
	/** Secondary Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Accuracy, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Accuracy;
	/** Secondary Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Evasion, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Evasion;
	/** Secondary Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitRate, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CriticalHitRate;
	/** Secondary Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitDamage, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CriticalHitDamage;

	/** Secondary Attribute - Bonus to attack, recast, and casting speeds. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Haste, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Haste;
	
	/** Secondary Attribute - To status and damage types. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Resistance, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Resistance;
	/** Secondary Attribute - The final multiplier for damage taken.*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_DamageTaken, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DamageTaken;

	/** Secondary Attribute */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MoveSpeed, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MoveSpeed;

	/** Secondary Attribute - A generic attribute for various events that need to check a percent. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Chance, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Chance;
	/** Secondary Attribute - Generic attribute for various bonuses. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Bonus, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Bonus;
};
