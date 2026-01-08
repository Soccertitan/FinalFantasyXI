// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attribute/HitPointsAttributeSet.h"
#include "CrysHitPointsAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UCrysHitPointsAttributeSet : public UHitPointsAttributeSet
{
	GENERATED_BODY()
	
public:
	UCrysHitPointsAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	ATTRIBUTE_ACCESSORS(UCrysHitPointsAttributeSet, BarrierPoints);
	
protected:

	UFUNCTION()
	void OnRep_BarrierPoints(const FGameplayAttributeData& OldValue);
	
	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void HandleDamage(const FGameplayEffectModCallbackData& Data, float Magnitude) override;
	
private:

	/**
	 * The current points that can be absorbed attribute. Damage will be inflicted to this attribute before being passed onto Health.
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BarrierPoints, Category = "Crim Ability System|HitPoints", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BarrierPoints;
};
