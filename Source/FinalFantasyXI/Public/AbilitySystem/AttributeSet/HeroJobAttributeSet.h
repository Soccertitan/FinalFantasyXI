// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "HeroJobAttributeSet.generated.h"

/**
 * An attribute set for the hero's jobs. Affects the base stats of the hero.
 */
UCLASS()
class FINALFANTASYXI_API UHeroJobAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UHeroJobAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(ThisClass, MainJobLevel);
	ATTRIBUTE_ACCESSORS(ThisClass, SubJobLevel);
	ATTRIBUTE_ACCESSORS(ThisClass, SubJobEffectiveness);
	
protected:
	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	UFUNCTION()
	void OnRep_MainJobLevel(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_SubJobLevel(const FGameplayAttributeData& OldValue);
	
private:
	
	/** MainJob Rank */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MainJobLevel, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MainJobLevel;
	
	/** SubJob Rank */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_SubJobLevel, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SubJobLevel;
	
	/** Increases the sub job stats. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SubJobEffectiveness;
};
