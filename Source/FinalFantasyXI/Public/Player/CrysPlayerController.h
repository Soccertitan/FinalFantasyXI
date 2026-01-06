// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "UINavPCReceiver.h"
#include "GameFramework/PlayerController.h"
#include "CrysPlayerController.generated.h"

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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UINavPCComponent);
	TObjectPtr<UUINavPCComponent> UINavPCComponent;

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

	UPROPERTY()
	bool bHudInitialized = false;
};
