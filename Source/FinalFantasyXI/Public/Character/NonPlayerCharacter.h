// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CrysCharacter.h"
#include "GameplayTagAssetInterface.h"
#include "NonPlayerCharacter.generated.h"

class UAutoAttackManagerComponent;
class UDefenderAttributeSet;
class UAbilityAttributeSet;
class UMovementAttributeSet;
class UCombatSkillAttributeSet;
class UAttackerAttributeSet;
class UAbilitySet;
class UHitPointsComponent;
class UCrimAbilitySystemComponent;
class UPrimaryAttributeSet;
class UManaPointsAttributeSet;
class UResourcePointsAttributeSet;
class UCrysHitPointsAttributeSet;

UCLASS(Blueprintable)
class FINALFANTASYXI_API ANonPlayerCharacter : public ACrysCharacter, public IGameplayTagAssetInterface,
	public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UCrysHitPointsAttributeSet> HitPointsAttributeSet;
	UPROPERTY()
	TObjectPtr<UResourcePointsAttributeSet> TacticalPointsAttributeSet;
	UPROPERTY()
	TObjectPtr<UManaPointsAttributeSet> ManaPointsAttributeSet;
	UPROPERTY()
	TObjectPtr<UPrimaryAttributeSet> PrimaryAttributeSet;
	UPROPERTY()
	TObjectPtr<UAttackerAttributeSet> AttackerAttributeSet;
	UPROPERTY()
	TObjectPtr<UDefenderAttributeSet> DefenderAttributeSet;
	UPROPERTY()
	TObjectPtr<UCombatSkillAttributeSet> CombatSkillAttributeSet;
	UPROPERTY()
	TObjectPtr<UMovementAttributeSet> MovementAttributeSet;
	UPROPERTY()
	TObjectPtr<UAbilityAttributeSet> AbilityAttributeSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitPointsComponent", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHitPointsComponent> HitPointsComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AutoAttackManagerComponent", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAutoAttackManagerComponent> AutoAttackManagerComponent;

public:
	ANonPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void PostInitializeComponents() override;
	
	// Implements IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	// End Implements IGameplayTagAssetInterface
	
	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// End Implements IAbilitySystemInterface

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilitySystem();
	
	/** Begins the death sequence for the character (disables collision, disables movement) */
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);

	/** Ends the death sequence for the character. */
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor) {}

	/** Begins the resurrection sequence for the character. */
	UFUNCTION()
	virtual void OnResurrectionStarted(AActor* OwningActor) {}

	/** Ends the resurrection sequence for the character. (enables collision, enables movement) */
	UFUNCTION()
	virtual void OnResurrectionFinished(AActor* OwningActor);
	
private:
	/** Abilities, attributes, and gameplay effects to grant. */
	UPROPERTY(EditDefaultsOnly, Category = "Character|Ability")
	TArray<TObjectPtr<UAbilitySet>> AbilitySets;
};
