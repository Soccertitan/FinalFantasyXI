// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/CrysGameplayAbility.h"
#include "ToggleCombatStanceGameplayAbility.generated.h"

class UAutoAttackManagerComponent;
class UCombatAnimationData;

/**
 * 
 */
UCLASS(Abstract)
class FINALFANTASYXI_API UToggleCombatStanceGameplayAbility : public UCrysGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "ToggleCombatState")
	UAnimMontage* GetPrimaryEnterCombatAnim() const;
	UFUNCTION(BlueprintPure, Category = "ToggleCombatState")
	UAnimMontage* GetPrimaryExitCombatAnim() const;
	
	UFUNCTION(BlueprintPure, Category = "ToggleCombatState")
	UAnimMontage* GetSecondaryEnterCombatAnim() const;
	UFUNCTION(BlueprintPure, Category = "ToggleCombatState")
	UAnimMontage* GetSecondaryExitCombatAnim() const;
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
private:
	/** Primary attack animations. */
	UPROPERTY(BlueprintReadOnly, Category = "ToggleCombatState", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCombatAnimationData> PrimaryAttacks;
	/** Attacks for the offhand when dual wielding. */
	UPROPERTY(BlueprintReadOnly, Category = "ToggleCombatState", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCombatAnimationData> SecondaryAttacks;
	
	UPROPERTY(BlueprintReadOnly, Category = "ToggleCombatState", meta = (AllowPrivateAccess = true))
	bool bDualWielding = false;
	
	UPROPERTY()
	TObjectPtr<UAutoAttackManagerComponent> AutoAttackManager;
	
	void InitAutoAttackManager(const FGameplayAbilityActorInfo* ActorInfo);
	
	UFUNCTION()
	void OnPrimaryAttacksChanged(UCombatAnimationData* AnimationData);
	UFUNCTION()
	void OnSecondaryAttacksChanged(UCombatAnimationData* AnimationData);
	
	void OnDualWieldingTagChanged(const FGameplayTag Tag, int32 NewCount);
};
