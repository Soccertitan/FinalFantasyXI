// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "CombatSkillAttributeSet.generated.h"

/**
 * The various skills that boost the effectiveness of abilities.
 */
UCLASS()
class FINALFANTASYXI_API UCombatSkillAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UCombatSkillAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	ATTRIBUTE_ACCESSORS(ThisClass, WeaponSkill);
	ATTRIBUTE_ACCESSORS(ThisClass, MagicSkill);
	ATTRIBUTE_ACCESSORS(ThisClass, EvasionSkill);
	
protected:
	
	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	UFUNCTION()
	void OnRep_WeaponSkill(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MagicSkill(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_EvasionSkill(const FGameplayAttributeData& OldValue);
	
private:
	
	/** Boost the effectiveness of weapons. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_WeaponSkill, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WeaponSkill;
	
	/** Boost the effectiveness of magic. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MagicSkill, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MagicSkill;
	
	/** Boost the likelihood of evading attacks. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_EvasionSkill, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData EvasionSkill;
};
