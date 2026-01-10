// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "ManaPointsAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UManaPointsAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
	public:

	UManaPointsAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	ATTRIBUTE_ACCESSORS(UManaPointsAttributeSet, CurrentPoints);
	ATTRIBUTE_ACCESSORS(UManaPointsAttributeSet, MaxPoints);
	ATTRIBUTE_ACCESSORS(UManaPointsAttributeSet, Healing);
	ATTRIBUTE_ACCESSORS(UManaPointsAttributeSet, Damage);

protected:

	UFUNCTION()
	void OnRep_CurrentPoints(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxPoints(const FGameplayAttributeData& OldValue);

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void HandleDamage(const FGameplayEffectModCallbackData& Data, float Magnitude);
	virtual void HandleHealing(const FGameplayEffectModCallbackData& Data, float Magnitude);

private:

	/**
	 * The current ManaPoints attribute. The value will be capped by the MaxManaPoints attribute.
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentPoints, Category = "Resource Attribute Set", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CurrentPoints;

	/**
	 * The current MaxManaPoints attribute. MaxManaPoints is an attribute since gameplay effects can modify it.
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxPoints, Category = "Resource Attribute Set", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxPoints;

	// -------------------------------------------------------------------
	//	Meta Attribute (please keep attributes that aren't 'stateful' below 
	// -------------------------------------------------------------------

	// Incoming healing. This is mapped directly to +CurrentPoints
	UPROPERTY(BlueprintReadOnly, Category="Resource Attribute Set", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Healing;

	// Incoming damage. This is mapped directly to -CurrentPoints
	UPROPERTY(BlueprintReadOnly, Category="Resource Attribute Set", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Damage;
};
