// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "InputActionViewModel.generated.h"

class UCrimEnhancedInputComponent;
class UInputAction;
class UInputActionListenerViewModel;
/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UInputActionViewModel : public UCrysViewModel
{
	GENERATED_BODY()
	
public:
	virtual void InitializeViewModel(APlayerController* PlayerController) override;
	
	/** Finds or creates an InputActionListener VM from an InputAction. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|InputAction")
	UInputActionListenerViewModel* FindOrCreateInputActionListenerViewModel(UInputAction* InputAction);
	
protected:
	void InitEnhancedInputComponent(APlayerController* PlayerController);
	
private:
	UPROPERTY()
	TObjectPtr<UCrimEnhancedInputComponent> EnhancedInputComponent;
	
	UPROPERTY()
	TArray<TObjectPtr<UInputActionListenerViewModel>> InputActionListenerViewModels;
	
	UInputActionListenerViewModel* InternalCreateInputActionListenerViewModel(UInputAction* InputAction);
};
