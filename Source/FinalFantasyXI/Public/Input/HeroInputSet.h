// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HeroInputSet.generated.h"

class UInputMappingContext;
class UInputAction;
class UCrimInputActionSet;

/**
 * InputActions for a hero.
 */
UCLASS()
class FINALFANTASYXI_API UHeroInputSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// Context for walking/running and changing the camera.
	// UPROPERTY(EditDefaultsOnly, Category = "Locomotion")
	// TObjectPtr<UInputMappingContext> ContextLocomotion;
	// UPROPERTY(EditDefaultsOnly, Category = "Locomotion")
	// int32 ContextLocomotionPriority = 0;
	//
	// UPROPERTY(EditDefaultsOnly, Category = "Locomotion|Action")
	// TObjectPtr<UInputAction> InputActionMovement;
	// UPROPERTY(EditDefaultsOnly, Category = "Locomotion|Action")
	// TObjectPtr<UInputAction> InputActionCamera;
	// UPROPERTY(EditDefaultsOnly, Category = "Locomotion|Action")
	// TObjectPtr<UInputAction> InputActionCameraZoomIn;
	// UPROPERTY(EditDefaultsOnly, Category = "Locomotion|Action")
	// TObjectPtr<UInputAction> InputActionCameraZoomOut;
	//
	// // The context containing all GameplayAbility actions.
	// UPROPERTY(EditDefaultsOnly, Category = "Ability")
	// TObjectPtr<UInputMappingContext> ContextAbility;
	// UPROPERTY(EditDefaultsOnly, Category = "Ability")
	// int32 ContextAbilityPriority = 10;
	
	// // These actions will trigger pressed events while InputActionAbilityToggle is pressed.
	// UPROPERTY(EditDefaultsOnly, Category = "Ability|Action")
	// TObjectPtr<UCrimInputActionSet> InputConfigPrimaryAbility;
	// // These actions will trigger pressed events while InputActionSubAbilityToggle is pressed.
	// UPROPERTY(EditDefaultsOnly, Category = "Ability|Action")
	// TObjectPtr<UCrimInputActionSet> InputConfigSubAbility;
	// // When pressed enables the input actions in InputConfigPrimaryAbility.
	// UPROPERTY(EditDefaultsOnly, Category = "Ability|Action")
	// TObjectPtr<UInputAction> InputActionPrimaryAbilityToggle;
	// // When pressed enables the input actions in InputConfigSubAbility.
	// UPROPERTY(EditDefaultsOnly, Category = "Ability|Action")
	// TObjectPtr<UInputAction> InputActionSubAbilityToggle;
	// // These InputActions can activate if both PrimaryAbilityToggle and SubAbilityToggle are not pressed.
	// UPROPERTY(EditDefaultsOnly, Category = "Ability|Action")
	// TObjectPtr<UCrimInputActionSet> InputConfigGenericAbility;
};
