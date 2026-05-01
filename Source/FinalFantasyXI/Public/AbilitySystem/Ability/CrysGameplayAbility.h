// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Ability/CrimGameplayAbility.h"
#include "AbilitySystem/AbilityTypes.h"
#include "CrysGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UCrysGameplayAbility : public UCrimGameplayAbility
{
	GENERATED_BODY()
	
public:
	UCrysGameplayAbility();
	
	UFUNCTION(BlueprintPure, Category = "Ability")
	EAbilityTargetType GetTargetType() const { return TargetType; }
	
	/** Returns the TargetRange scaled at ability level. */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Ability")
	float GetTargetRange() const;
	
	/** Returns the TargetHalfAngle scaled at ability level. */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Ability")
	float GetTargetHalfAngle() const;
	
	/** Returns the CastTime scaled at ability level multiplied by the CastTime multiplier evaluated with tags. */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Ability")
	float GetCastTime() const;
	
	/** Returns true if the target is within range and the half angle, the correct TargetType, and meets the TargetTagRequirements. */
	UFUNCTION(BlueprintPure, Category = "Ability")
	bool IsTargetValid(AActor* TargetActor) const;
	
	/** Returns true if the target meets the AbilityTargetType requirement. */
	UFUNCTION(BlueprintPure, Category = "Ability")
	bool IsTargetOfAbilityTargetType(AActor* TargetActor) const;
	
	/** Returns an array of Actors that is the appropriate TargetType and within the cone. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Ability")
	TArray<AActor*> GetValidTargetsInCone(const FVector Origin, const FVector ForwardVector, const float HalfAngle, const float Distance);

protected:
	/** Used in Apply GameplayEffectToTarget to automatically set the SetByCaller magnitudes. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	TMap<FGameplayTag, FScalableFloat> SetByCallerMap;
	
	/** Applies a GE to the target actor. Setting the Tag for SetByCaller values. */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Ability")
	void ApplyGameplayEffectToTarget(AActor* TargetActor, AActor* EffectCauser = nullptr);
	
private:
	/** Defines who the ability can target. */
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	EAbilityTargetType TargetType;
	
	/** The initial target must be within range to hit. */
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FScalableFloat TargetRange;
	
	/** The target must be within the half angle to be affected. */
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FScalableFloat TargetHalfAngle;
	
	/** The time to wait before an ability effects targets. */
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FScalableFloat CastTime;
	
	/** The target must meet these conditions when checking IsTargetValid */
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FGameplayTagRequirements TargetTagRequirements;
};
