// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/StreamableManager.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "HeroManagerViewModel.generated.h"

struct FHeroJobProgressItem;
class UHeroJobViewModel;
class UHeroJobDefinition;
class UHeroManagerComponent;

/**
 * Allows the switching of the HeroJob in the HeroManagerComponent and has information of the HeroJobProgressItems.
 */
UCLASS(Abstract)
class FINALFANTASYXI_API UHeroManagerViewModel : public UCrysViewModel
{
	GENERATED_BODY()

public:
	bool GetIsLoadingHeroJobs() const { return bLoadingHeroJobs; }
	bool GetIsSwitchingHeroJobs() const { return bSwitchingHeroJobs; }

	UFUNCTION(BlueprintPure, FieldNotify)
	UHeroJobViewModel* GetHeroMainJobViewModel() const { return HeroMainJobViewModel; }
	UFUNCTION(BlueprintPure, FieldNotify)
	UHeroJobViewModel* GetHeroSubJobViewModel() const { return HeroSubJobViewModel; }

	/** Finds a ViewModel with the specified HeroJobTag. */
	UFUNCTION(BlueprintPure, Category = "HeroManager ViewModel")
	UHeroJobViewModel* FindHeroJobViewModel(UPARAM(meta = (Categories = "HeroJob")) FGameplayTag HeroJobTag);

	/** Tries to switch to specified HeroJob. */
	UFUNCTION(BlueprintCallable, Category = "HeroManager ViewModel")
	void TrySetHeroJobs(UPARAM(meta = (Categories = "HeroJob")) FGameplayTag MainJobTag, UPARAM(meta = (Categories = "HeroJob")) FGameplayTag SubJobTag);

protected:
	virtual void OnInitializeViewModel(APlayerController* PlayerController) override;

	void SetIsLoadingHeroJobs(const bool InValue);
	void SetHeroMainJobViewModel(UHeroJobViewModel* InValue);
	void SetHeroSubJobViewModel(UHeroJobViewModel* InValue);

private:
	/** The HeroJobDefinition to load. */
	UPROPERTY(EditAnywhere, Category = "HeroManagerViewModel", meta = (AllowedTypes = "HeroJobDefinition"))
	TArray<FPrimaryAssetId> HeroJobsToLoad;

	/** Cached pointer to the HeroManagerComponent from the PlayerState. */
	UPROPERTY()
	TObjectPtr<UHeroManagerComponent> HeroManagerComponent;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter = "GetIsLoadingHeroJobs", meta = (AllowPrivateAccess = "true"))
	bool bLoadingHeroJobs = true;

	/** True if waiting to switch HeroJobs. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter = "GetIsSwitchingHeroJobs", meta = (AllowPrivateAccess = "true"))
	bool bSwitchingHeroJobs = false;

	/** Cached handle for the HeroJobDefinition. */
	TSharedPtr<FStreamableHandle> HeroJobStreamableHandle;

	/** The current HeroMainJob the player is. */
	UPROPERTY()
	TObjectPtr<UHeroJobViewModel> HeroMainJobViewModel;
	
	/** The current HeroSubJob the player is. */
	UPROPERTY()
	TObjectPtr<UHeroJobViewModel> HeroSubJobViewModel;

	/** All the HeroJob ViewModels created from the HeroJobsToLoad. */
	UPROPERTY()
	TArray<TObjectPtr<UHeroJobViewModel>> HeroJobViewModels;
	
	void LoadHeroJobs();
	/** Creates ViewModels for each loaded hero class.*/
	void OnHeroJobsLoaded();

	void CreateHeroJobViewModel(UHeroJobDefinition* HeroJob);

	UFUNCTION()
	void OnHeroMainJobChanged(UHeroManagerComponent* InHeroManagerComponent);
	
	UFUNCTION()
	void OnHeroSubJobChanged(UHeroManagerComponent* InHeroManagerComponent);

	UFUNCTION()
	void OnTrySetHeroJob(UHeroManagerComponent* InHeroManagerComponent, bool bSuccess);

	UFUNCTION()
	void OnHeroJobProgressUpdated(UHeroManagerComponent* InHeroManagerComponent, const FHeroJobProgressItem& HeroJobProgressItem);
};
