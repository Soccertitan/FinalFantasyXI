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
	virtual void InitializeViewModel(APlayerController* PlayerController);

protected:
	
	/**
	 * Called when the view model is ready to be initialized. Called after the C++ version.
	 * PlayerController is guaranteed to be valid.
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "InitializeViewModel")
	void K2_OnInitializeViewModel(APlayerController* PlayerController);
	
	UFUNCTION()
	virtual void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnPossessedPawnChanged")
	void K2_OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
};
