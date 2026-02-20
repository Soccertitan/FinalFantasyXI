// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/CrysGameplayAbility.h"
#include "CombatGameplayAbility.generated.h"

class UCombatAnimationData;

/**
 * An ability that listens for changes to the primary and secondary combat animations.
 */
UCLASS(Abstract)
class FINALFANTASYXI_API UCombatGameplayAbility : public UCrysGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Ability|Combat")
	UCombatAnimationData* GetPrimaryCombatAnimationData() const;
	
	UFUNCTION(BlueprintPure, Category = "Ability|Combat")
	UCombatAnimationData* GetSecondaryCombatAnimationData() const;
	
	UFUNCTION(BlueprintPure, Category = "Ability|Combat")
	bool IsDualWielding() const { return bDualWielding;}
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

private:
	/** Primary attack animations retrieved from the CrysCharacter. */
	UPROPERTY(BlueprintReadOnly, Category = "Ability|Combat", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCombatAnimationData> PrimaryCombatAnimationData;
	/** Attacks for the offhand when dual wielding. */
	UPROPERTY(BlueprintReadOnly, Category = "Ability|Combat", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCombatAnimationData> SecondaryCombatAnimationData;

	bool bDualWielding = false;
	
	UFUNCTION()
	void OnPrimaryAnimationDataChanged(UCombatAnimationData* AnimationData);
	UFUNCTION()
	void OnSecondaryAnimationDataChanged(UCombatAnimationData* AnimationData);
};
