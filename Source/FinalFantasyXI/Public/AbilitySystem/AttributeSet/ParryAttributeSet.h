// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "ParryAttributeSet.generated.h"

/**
 * Attributes related to parrying.
 */
UCLASS()
class FINALFANTASYXI_API UParryAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UParryAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(ThisClass, ParrySkill);
	ATTRIBUTE_ACCESSORS(ThisClass, ParryChance);
	ATTRIBUTE_ACCESSORS(ThisClass, ParryAngle);
	
protected:
	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	UFUNCTION()
	void OnRep_ParrySkill(const FGameplayAttributeData& OldValue);
	
private:
	
	/** Boost the likelihood of parrying. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ParrySkill, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ParrySkill;
	
	/** The likelihood of parrying. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ParryChance;
	
	/** The front angle valid for parrying. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ParryAngle;
};
