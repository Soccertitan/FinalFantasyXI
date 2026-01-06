// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrysViewModel.h"
#include "GameplayTagContainer.h"
#include "Engine/StreamableManager.h"
#include "HeroManagerViewModel.generated.h"

struct FHeroClassProgressItem;
class UHeroClassViewModel;
class UHeroClassDefinition;
class UHeroManagerComponent;

/**
 * Allows the switching of the HeroClass in the HeroManagerComponent and has information of the HeroClassProgressItems.
 */
UCLASS(Abstract)
class FINALFANTASYXI_API UHeroManagerViewModel : public UCrysViewModel
{
	GENERATED_BODY()

public:
	bool GetIsLoadingHeroClasses() const { return bLoadingHeroClasses; }
	bool GetIsSwitchingHeroClass() const { return bSwitchingHeroClass; }

	UFUNCTION(BlueprintPure, FieldNotify)
	UHeroClassViewModel* GetCurrentHeroClassViewModel() const { return CurrentHeroClassViewModel; }

	/** Finds a ViewModel with the specified HeroClassTag. */
	UFUNCTION(BlueprintPure, Category = "HeroManager ViewModel", meta = (Categories = "HeroClass"))
	UHeroClassViewModel* FindHeroClassViewModel(FGameplayTag HeroClassTag);

	/** Tries to switch to specified HeroClass. */
	UFUNCTION(BlueprintCallable, Category = "HeroManager ViewModel", meta = (Categories = "HeroClass"))
	void TrySetHeroClass(FGameplayTag HeroClassTag);

protected:
	virtual void OnInitializeViewModel(APlayerController* PlayerController) override;

	void SetIsLoadingHeroClasses(const bool InValue);
	void SetCurrentHeroClassViewModel(UHeroClassViewModel* InValue);

private:
	/** The HeroClassDefinition to load. */
	UPROPERTY(EditAnywhere, Category = "HeroManagerViewModel", meta = (AllowedTypes = "HeroClassDefinition"))
	TArray<FPrimaryAssetId> HeroClassesToLoad;

	/** Cached pointer to the HeroManagerComponent from the PlayerState. */
	UPROPERTY()
	TObjectPtr<UHeroManagerComponent> HeroManagerComponent;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter = "GetIsLoadingHeroClasses", meta = (AllowPrivateAccess = "true"))
	bool bLoadingHeroClasses = true;

	/** True if waiting to switch HeroClasses. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter = "GetIsSwitchingHeroClass", meta = (AllowPrivateAccess = "true"))
	bool bSwitchingHeroClass = false;

	/** Cached handle for the HeroClassDefinition. */
	TSharedPtr<FStreamableHandle> HeroClassStreamableHandle;

	/** The current HeroClass the player is. */
	UPROPERTY()
	TObjectPtr<UHeroClassViewModel> CurrentHeroClassViewModel;

	/** All the HeroClass ViewModels created from the HeroClassesToLoad. */
	UPROPERTY()
	TArray<TObjectPtr<UHeroClassViewModel>> HeroClassViewModels;
	
	void LoadHeroClasses();
	/** Creates ViewModels for each loaded hero class.*/
	void OnHeroClassesLoaded();

	void CreateHeroClassViewModel(UHeroClassDefinition* HeroClass);

	UFUNCTION()
	void OnHeroClassChanged(UHeroManagerComponent* InHeroManagerComponent);

	UFUNCTION()
	void OnTrySetHeroClassFailed(UHeroManagerComponent* InHeroManagerComponent);

	UFUNCTION()
	void OnHeroClassProgressUpdated(UHeroManagerComponent* InHeroManagerComponent, const FHeroClassProgressItem& HeroClassProgressItem);
};
