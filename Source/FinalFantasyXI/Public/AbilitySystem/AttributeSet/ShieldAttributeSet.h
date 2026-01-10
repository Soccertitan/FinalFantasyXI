// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "ShieldAttributeSet.generated.h"

/**
 * Defines the attributes related to blocking with a shield.
 */
UCLASS()
class FINALFANTASYXI_API UShieldAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UShieldAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(ThisClass, ShieldSkill);
	ATTRIBUTE_ACCESSORS(ThisClass, BlockChance);
	ATTRIBUTE_ACCESSORS(ThisClass, BlockAngle);
	ATTRIBUTE_ACCESSORS(ThisClass, BlockDamageReduction);
	
protected:
	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	UFUNCTION()
	void OnRep_ShieldSkill(const FGameplayAttributeData& OldValue);
	
private:
	
	/** Boost the likelihood of blocking. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ShieldSkill, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ShieldSkill;
	
	/** The likelihood of blocking. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BlockChance;
	
	/** The front angle valid for blocking. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BlockAngle;
	
	/** The percent reduction in taking damage from blocking. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BlockDamageReduction;
};
