// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ViewModelBlueprintFunctionLibrary.generated.h"

class UCrysViewModel;

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UViewModelBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * If a ViewModel of class already exists returns that ViewModel. Otherwise, creates a new ViewModel of
	 * the class type. Then initializes the ViewModel with the owning HUD's PlayerController.
	 */
	UFUNCTION(BlueprintCallable, Category = "CrysFunctionLibrary|ViewModel", meta=(DeterminesOutputType=ViewModelClass))
	static UCrysViewModel* FindOrCreateViewModel(const TSubclassOf<UCrysViewModel> ViewModelClass, APlayerController* PlayerController);
};
