// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InteractorInterface.h"
#include "InventorySystemInterface.h"
#include "TargetingSystemInterface.h"
#include "CrysCharacter.h"
#include "GameplayTagAssetInterface.h"
#include "AbilitySystem/AbilityTargetInterface.h"
#include "EquipmentSystem/EquipmentSystemInterface.h"
#include "HeroSystem/HeroSystemInterface.h"
#include "HeroCharacter.generated.h"

class UHitPointsComponent;
class UCrimAbilitySystemComponent;
class UAbilitySet;
class UInteractorBoxComponent;
class UInteractorComponent;
class UTargetingSystemComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS(Blueprintable)
class FINALFANTASYXI_API AHeroCharacter : public ACrysCharacter, public IInventorySystemInterface,
	public ITargetingSystemInterface, public IInteractorInterface, public IHeroSystemInterface, public IEquipmentSystemInterface, 
	public IAbilitySystemInterface, public IGameplayTagAssetInterface, public IAbilityTargetInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	/** Allows the character to target and lock on to other actors. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TargetSystem, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTargetingSystemComponent> TargetingSystemComponent;

	/** Allows character to interact with an interactable. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interactor, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInteractorComponent> InteractorComponent;
	
	/** Allows character to interact with an interactable box component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = InteractorBox, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInteractorBoxComponent> InteractorBoxComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HitPointsComponent, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHitPointsComponent> HitPointsComponent;

public:
	AHeroCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// End Implements IAbilitySystemInterface
	
	// Implements AbilityTargetInterface
	virtual AActor* GetAbilityTarget_Implementation(const FGameplayTagContainer& AbilityTags) const override;
	// End Implements AbilityTargetInterface
	
	// Implements IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	// End Implements IGameplayTagAssetInterface

	virtual UInventoryManagerComponent* GetInventoryManagerComponent_Implementation() const override;
	virtual UTargetingSystemComponent* GetTargetingSystemComponent_Implementation() const override;
	virtual UInteractorComponent* GetInteractorComponent_Implementation() const override;
	virtual UHeroManagerComponent* GetHeroManagerComponent_Implementation() const override;
	virtual UEquipmentManagerComponent* GetEquipmentManagerComponent_Implementation() const override;

protected:

	void InitAbilitySystem();
	
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
	
	/** Cached ASC from the PlayerState. */
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
};
