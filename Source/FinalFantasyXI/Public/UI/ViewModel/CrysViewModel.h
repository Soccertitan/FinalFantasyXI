// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Types/MVVMEventField.h"
#include "CrysViewModel.generated.h"

/**
 * The base ViewModel that takes a PlayerController and spawns other supporting ViewModels.
 */
UCLASS(Abstract)
class FINALFANTASYXI_API UCrysViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	
	/**
	 * If a valid PlayerController is passed in it will call OnInitializeViewModel.
	 */
	void InitializeViewModel(APlayerController* PlayerController);

	UFUNCTION(BlueprintPure, FieldNotify)
	FMVVMEventField OnViewModelInitialized() const { return{}; }

protected:

	/**
	 * Called when the view model is ready to be initialized. PlayerController is guaranteed to be valid.  
	 */
	virtual void OnInitializeViewModel(APlayerController* PlayerController) {}
	
	/**
	 * Called when the view model is ready to be initialized. Called after the C++ version.
	 * PlayerController is guaranteed to be valid.
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnInitializeViewModel")
	void K2_OnInitializeViewModel(APlayerController* PlayerController);
};
