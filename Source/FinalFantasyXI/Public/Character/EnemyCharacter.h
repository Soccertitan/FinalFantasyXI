// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CrysCharacter.h"
#include "GameplayTagAssetInterface.h"
#include "EnemyCharacter.generated.h"

class UAbilitySet;
class UHitPointsComponent;
class UCrimAbilitySystemComponent;
class UCrysAttributeSet;
class UManaPointsAttributeSet;
class UTacticalPointsAttributeSet;
class UCrysHitPointsAttributeSet;

UCLASS(Blueprintable)
class FINALFANTASYXI_API AEnemyCharacter : public ACrysCharacter, public IGameplayTagAssetInterface,
	public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySystem, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UCrysHitPointsAttributeSet> HitPointsAttributeSet;
	UPROPERTY()
	TObjectPtr<UTacticalPointsAttributeSet> TacticalPointsAttributeSet;
	UPROPERTY()
	TObjectPtr<UManaPointsAttributeSet> ManaPointsAttributeSet;
	
	UPROPERTY()
	TObjectPtr<UCrysAttributeSet> CrysAttributeSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HitPointsComponent, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHitPointsComponent> HitPointsComponent;

public:
	AEnemyCharacter(const FObjectInitializer& ObjectInitializer);
	
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
