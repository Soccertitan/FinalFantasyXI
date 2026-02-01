// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "UINavPCReceiver.h"
#include "GameFramework/PlayerController.h"
#include "CrysPlayerController.generated.h"

class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
class UInputActionListenerMap;
class UWidget;
class UUINavPCComponent;

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API ACrysPlayerController : public APlayerController, public IUINavPCReceiver
{
	GENERATED_BODY()

public:
	ACrysPlayerController();
	virtual void SetupInputComponent() override;

	/**
	 * Creates a widget and then adds it to the Viewport. If a UINavWidget already exists, removes it and all parents
	 * from the viewport.
	 * @param WidgetClass The WidgetClass to create and add to the Viewport.
	 * @param bInRestrictMovement If true, will disable move and look input.
	 */
	UFUNCTION(BlueprintCallable, Category = "PlayerController|Crys")
	void CreateUINavWidgetAndAddToViewport(TSubclassOf<UUINavWidget> WidgetClass, bool bInRestrictMovement = true);

	/**
	 * Adds the specified UINavWidget to the viewport. Handles setting up the input mode and focus. If a UINavWidget
	 * already exists, remove it and all parents from the viewport.
	 * @param InWidget The UINav Widget to add to the viewport.
	 * @param InWidgetToFocus The widget to focus on.
	 * @param bInRestrictMovement If true, will disable move and look input.
	 */
	UFUNCTION(BlueprintCallable, Category = "PlayerController|Crys")
	void AddUINavWidgetToViewport(UUINavWidget* InWidget, UWidget* InWidgetToFocus, bool bInRestrictMovement = true);

	// UINavPCReceiver Interface
	virtual void OnRootWidgetAdded_Implementation() override;
	virtual void OnRootWidgetRemoved_Implementation() override;
	// ~UINavPCReceiver Interface

	/** Adds the Inputs in the DefaultInputActionListeners to the CrimEnhancedInputComponent. */
	void ApplyDefaultInputActionMap();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UINavPCComponent);
	TObjectPtr<UUINavPCComponent> UINavPCComponent;
	UPROPERTY()
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> EnhancedInputSubsystem;

	/**
	 * Calls InitializeHUD() from the DreamHUD class. This will only call it once, setting up the widget and ViewModel.
	 * Call this when ready to set up the HUD.
	 */
	UFUNCTION(BlueprintCallable)
	void InitializeHUD();

private:
	/**
	 * Restricts movement and look input when a UINavWidget is added to the viewport.
	 */
	UPROPERTY()
	bool bRestrictMovement = true;
	
	/** The InputMappingContexts to apply at startup with the integer as the priority. */
	UPROPERTY(EditAnywhere, Category = "CrysPlayerController", meta = (NoResetToDefault))
	TMap<TObjectPtr<UInputMappingContext>, int32> InputMappingContextMap;
	/** Binds the Listeners to the InputActions during InputComponent Setup. */
	UPROPERTY(EditAnywhere, Category = "CrysPlayerController", meta = (NoResetToDefault))
	TArray<TObjectPtr<UInputActionListenerMap>> DefaultInputActionListeners;

	UPROPERTY()
	bool bHudInitialized = false;
};
