// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "JobAttributeSet.generated.h"

/**
 * An attribute set for the hero's jobs. Affects the base stats of the hero.
 */
UCLASS()
class FINALFANTASYXI_API UJobAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UJobAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(ThisClass, MainJobRank);
	ATTRIBUTE_ACCESSORS(ThisClass, SubJobRank);
	ATTRIBUTE_ACCESSORS(ThisClass, SubJobEffectiveness);
	
protected:
	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	UFUNCTION()
	void OnRep_MainJobRank(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_SubJobRank(const FGameplayAttributeData& OldValue);
	
private:
	
	/** MainJob Rank */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MainJobRank, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MainJobRank;
	
	/** SubJob Rank */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_SubJobRank, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SubJobRank;
	
	/** Increases the sub job stats. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SubJobEffectiveness;
};
