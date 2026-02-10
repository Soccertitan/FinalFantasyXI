// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "../CrysPlayerController.h"
#include "HeroPlayerController.generated.h"

class UCrysActionManagerComponent;
class UCrimInputActionSet;
class UCrimEnhancedInputComponent;
struct FInputActionValue;
struct FGameplayTag;
class UHeroInputSet;
class UCrimAbilitySystemComponent;
class UTargetingSystemComponent;
class UEnhancedInputLocalPlayerSubsystem;
class UTargetPointFilterBase;
class UAbilityInputManagerComponent;

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API AHeroPlayerController : public ACrysPlayerController
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilityInputManager", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityInputManagerComponent> AbilityInputManagerComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ActionManagerComponent", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCrysActionManagerComponent> ActionManagerComponent;

public:
	AHeroPlayerController();

	//-----------------------------------------------------------------------------------------
	// Class overrides
	//-----------------------------------------------------------------------------------------
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void AcknowledgePossession(class APawn* P) override;
	virtual void OnRep_PlayerState() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	// UINavPCReceiver Interface
	// virtual void OnRootWidgetAdded_Implementation() override;
	// virtual void OnRootWidgetRemoved_Implementation() override;
	// ~UINavPCReceiver Interface

protected:
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Hero")
	TArray<TObjectPtr<UTargetPointFilterBase>> TargetFilters;

private:
	/** Cached reference to the PlayerStates ASC. */
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	/** Cached references to the Pawns TargetSystemComponent. */
	UPROPERTY()
	TObjectPtr<UTargetingSystemComponent> TargetSystemComponent;

	/** Tries to initialize the overlay if the Pawn and PlayerState are available. */
	void TryInitOverlay();
	
	/**
	 *  Retrieve the Ability System Component from the PlayerState and caches it locally. And initializing
	 *  other components that need a reference to the ASC.
	 */
	void InitializeAbilitySystemComponent();

	/** Retrieves the TargetSystemComponent from the pawn. */
	void InitializeTargetSystemComponent();

//-------------------------------------------------------------------
// Enhanced Input Functionality
//-------------------------------------------------------------------

};
