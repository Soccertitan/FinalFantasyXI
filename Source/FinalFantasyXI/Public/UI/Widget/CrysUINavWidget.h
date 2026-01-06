// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "UINavWidget.h"
#include "CrysUINavWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrysUINavOnReturnToWidgetSignature, UCrysUINavWidget*, PreviousActiveWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrysUINavOnReturnFromWidgetSignature, UCrysUINavWidget*, NewActiveWidget);

/**
 * 
 */
UCLASS(Abstract)
class FINALFANTASYXI_API UCrysUINavWidget : public UUINavWidget
{
	GENERATED_BODY()

public:
	/**
	 * Adds/updates a key to ReturnToWidget Map.
	 * @param Key The Currently navigated Widget.
	 * @param Value The widget the Key should navigate to.
	 * @note You should call this in PreConstruct or during play to add key/value pairs.
	 */
	UFUNCTION(BlueprintCallable, Category = "Crys UINav")
	void SetReturnToWidgetMap(UCrysUINavWidget* Key, UCrysUINavWidget* Value);

	/**
	 * Removes the key value from the ReturnToWidgetMap
	 * @param Key The widget to remove from the map.
	 */
	UFUNCTION(BlueprintCallable, Category = "Crys UINav")
	void RemoveReturnToWidgetMap(UCrysUINavWidget* Key);

	/** Clears all Key/Value pairs in the ReturnToWidgetMap. */
	UFUNCTION(BlueprintCallable, Category = "Crys UINav")
	void ClearReturnToWidgetMap();

	/** Returns to the Widget based on the ReturnToWidgetMap. */
	UFUNCTION(BlueprintCallable, Category = "Crys UINav")
	void ReturnToWidgetInMap();

	/**
	*	Called when ReturnToParent is called (i.e. the player wants to exit the menu) and the ReturnToWidgetChain
	*	does not have a valid entry.
	*	@note Use this event instead of the UINavWidget OnReturn event.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = CrysUINavWidget, DisplayName = CrysOnReturn)
	void UINavOnReturn();
	virtual void UINavOnReturn_Implementation();

	/**
	 * Called when the focus changes to a different widget during the OnReturn event. This is called on the parent widget
	 * during the OnReturn event when a value is found in the ReturnToWidgetMap.
	 * @param PreviousActiveWidget The widget we returned from.
	 * @param NewActiveWidget The widget we focused on.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = CrysUINavWidget, DisplayName = OnReturnFromMap)
	void UINavOnReturnFromMap(UCrysUINavWidget* PreviousActiveWidget, UCrysUINavWidget* NewActiveWidget);

	/** Called on the Widget that was returned to in the ReturnToWidgetMap. */
	UPROPERTY(BlueprintAssignable, Category = CrysUINavWidget, DisplayName = OnReturnToWidget)
	FCrysUINavOnReturnToWidgetSignature OnReturnToWidgetDelegate;

	/**
	 * Called on the Widget that was returned to in the ReturnToWidgetMap.
	 * @param PreviousActiveWidget The widget that was returned from.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = CrysUINavWidget, DisplayName = OnReturnToWidget)
	void K2_UINavOnReturnToWidget(UCrysUINavWidget* PreviousActiveWidget);

	/** Called on the Widget that was returned from in the ReturnToWidgetMap. */
	UPROPERTY(BlueprintAssignable, Category = CrysUINavWidget, DisplayName = OnReturnFromWidget)
	FCrysUINavOnReturnFromWidgetSignature OnReturnFromWidgetDelegate;

	/**
	 * Called on the Widget that was returned from in the ReturnToWidgetMap.
	 * @param NewActiveWidget The widget that was returned to.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = CrysUINavWidget, DisplayName = OnReturnFromWidget)
	void K2_UINavOnReturnFromWidget(UCrysUINavWidget* NewActiveWidget);

	/** Set focus on the most recent entry added to ReturnToWidgetChain. */
	virtual void OnReturn_Implementation() override;

protected:
	virtual void UINavOnReturnToWidget(UCrysUINavWidget* PreviousActiveWidget);
	virtual void UINavOnReturnFromWidget(UCrysUINavWidget* NewActiveWidget);
	
private:
	/** The Active Widget (key) that is mapped to a widget to return to (value). */
	UPROPERTY()
	TMap<TObjectPtr<UCrysUINavWidget>, TObjectPtr<UCrysUINavWidget>> ReturnToWidgetMap;
};
