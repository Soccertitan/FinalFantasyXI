// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ViewModelBlueprintFunctionLibrary.generated.h"

class UCrysGameplayTagViewModel;
struct FItemInstance;
class UItemInstanceViewModel;
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
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|CrysFunctionLibrary", meta=(DeterminesOutputType=ViewModelClass))
	static UCrysViewModel* FindOrCreateViewModel(const TSubclassOf<UCrysViewModel> ViewModelClass, APlayerController* PlayerController);
	
	UFUNCTION(BlueprintPure, Category = "Utilities|Text", meta = (DisplayName = "To Text (GameplayTagViewModel - Long)"))
	static FText Combine_GameplayTagViewModelsText(const TArray<UCrysGameplayTagViewModel*> ViewModels, const FText Divider, const FText EmptyText);
	
	UFUNCTION(BlueprintPure, Category = "Utilities|Text", meta = (DisplayName = "To Text (GameplayTagViewModel - Short)"))
	static FText Combine_GameplayTagViewModelsShortText(const TArray<UCrysGameplayTagViewModel*> ViewModels, const FText Divider, const FText EmptyText);
};
