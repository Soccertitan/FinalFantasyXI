// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "../CrysPlayerController.h"
#include "HeroPlayerController.generated.h"

struct FInputActionValue;
struct FGameplayTag;
class UHeroInputSet;
class UCrimAbilitySystemComponent;
class UTargetingSystemComponent;
class UEnhancedInputLocalPlayerSubsystem;
class UTargetPointFilterBase;
class UAbilityInputManagerComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FHeroPlayerControllerBoolSignature, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FHeroPlayerControllerInputSignature, const FGameplayTag& /*InputTag*/);

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API AHeroPlayerController : public ACrysPlayerController
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilityInputManager, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityInputManagerComponent> AbilityInputManagerComponent;

public:
	AHeroPlayerController();

	/** Called when an InputTag is pressed. */
	FHeroPlayerControllerInputSignature OnInputPressedDelegate;
	/** Called when an InputTag is released. */
	FHeroPlayerControllerInputSignature OnInputReleasedDelegate;
	
	/** Called when the InputAction PrimaryAbilityToggle is pressed or released. */
	FHeroPlayerControllerBoolSignature OnPrimaryAbilityToggleStateChangedDelegate;
	/** Called when the InputAction SubAbilityToggle is pressed or released. */
	FHeroPlayerControllerBoolSignature OnSubAbilityToggleStateChangedDelegate;
	
	UFUNCTION(BlueprintPure, Category = "PlayerController|Hero")
	bool IsPrimaryAbilityTogglePressed() const { return bPrimaryAbilityTogglePressed; }
	UFUNCTION(BlueprintPure, Category = "PlayerController|Hero")
	bool IsSubAbilityTogglePressed() const { return bSubAbilityTogglePressed; }

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
	virtual void OnRootWidgetAdded_Implementation() override;
	virtual void OnRootWidgetRemoved_Implementation() override;
	// ~UINavPCReceiver Interface

protected:
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Hero")
	TArray<TObjectPtr<UTargetPointFilterBase>> TargetFilters;

	/**
	 * Removes the Ability Input Context and iterates through the HeroInputConfig sending a release input command
	 * for every ability input.
	 */
	void RemoveAbilityMappingContextAndReleaseAbilityInputs();

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
private:
	UPROPERTY(EditDefaultsOnly, Category = "Hero")
	TObjectPtr<UHeroInputSet> HeroInputSet;
	UPROPERTY()
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> EnhancedInputSubsystem;

	UPROPERTY()
	bool bPrimaryAbilityTogglePressed = false;
	UPROPERTY()
	bool bSubAbilityTogglePressed = false;

	/** Binds the input in the HeroInputSet to the EnhancedInputSubsystem. */
	void SetupHeroInputSet();

	void InputActionMovement(const FInputActionValue& Value);
	void InputActionCamera(const FInputActionValue& Value);
	void InputActionCameraZoomIn(const FInputActionValue& Value);
	void InputActionCameraZoomOut(const FInputActionValue& Value);

	void InputActionPrimaryAbility(const FInputActionValue& Value, FGameplayTag InputTag);
	void InputActionSubAbility(const FInputActionValue& Value, FGameplayTag InputTag);
	void InputActionGenericAbility(const FInputActionValue& Value, FGameplayTag InputTag);

	void InputActionPrimaryAbilityToggle(const FInputActionValue& Value);
	void InputActionSubAbilityToggle(const FInputActionValue& Value);

	void OnAbilityInputPressed(FGameplayTag InputTag);
	void OnAbilityInputReleased(FGameplayTag InputTag);
};
