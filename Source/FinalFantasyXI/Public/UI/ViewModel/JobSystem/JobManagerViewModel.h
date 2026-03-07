// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/StreamableManager.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "JobManagerViewModel.generated.h"

struct FJobProgressItem;
class UJobViewModel;
class UJobDefinition;
class UJobManagerComponent;

/**
 * Allows the switching of the Jobs in the JobManagerComponent and has information of the JobProgressItems.
 */
UCLASS(Abstract)
class FINALFANTASYXI_API UJobManagerViewModel : public UCrysViewModel
{
	GENERATED_BODY()

public:
	bool GetIsLoadingJobs() const { return bLoadingJobs; }
	bool GetIsSwitchingJobs() const { return bSwitchingJobs; }

	UFUNCTION(BlueprintPure, FieldNotify)
	UJobViewModel* GetMainJobViewModel() const { return MainJobViewModel; }
	UFUNCTION(BlueprintPure, FieldNotify)
	UJobViewModel* GetSubJobViewModel() const { return SubJobViewModel; }

	/** Finds a ViewModel with the specified JobTag. */
	UFUNCTION(BlueprintPure, Category = "Viewmodel|Job")
	UJobViewModel* FindJobViewModel(UPARAM(meta = (Categories = "Job")) FGameplayTag JobTag);

	/** Tries to switch to specified Job. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Job")
	void TrySetJobs(UPARAM(meta = (Categories = "Job")) FGameplayTag MainJobTag, UPARAM(meta = (Categories = "Job")) FGameplayTag SubJobTag);

protected:
	virtual void OnInitializeViewModel(APlayerController* PlayerController) override;

	void SetIsLoadingJobs(const bool InValue);
	void SetMainJobViewModel(UJobViewModel* InValue);
	void SetSubJobViewModel(UJobViewModel* InValue);

private:
	/** The JobDefinition to load. */
	UPROPERTY(EditAnywhere, Category = "JobManagerViewModel", meta = (AllowedTypes = "JobDefinition"))
	TArray<FPrimaryAssetId> JobsToLoad;

	/** Cached pointer to the JobManagerComponent from the PlayerState. */
	UPROPERTY()
	TObjectPtr<UJobManagerComponent> JobManagerComponent;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter = "GetIsLoadingJobs", meta = (AllowPrivateAccess = "true"))
	bool bLoadingJobs = true;

	/** True if waiting to switch Jobs. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter = "GetIsSwitchingJobs", meta = (AllowPrivateAccess = "true"))
	bool bSwitchingJobs = false;

	/** Cached handle for the JobDefinition. */
	TSharedPtr<FStreamableHandle> JobStreamableHandle;

	/** The current MainJob the player is. */
	UPROPERTY()
	TObjectPtr<UJobViewModel> MainJobViewModel;
	
	/** The current SubJob the player is. */
	UPROPERTY()
	TObjectPtr<UJobViewModel> SubJobViewModel;

	/** All the Job ViewModels created from the JobsToLoad. */
	UPROPERTY()
	TArray<TObjectPtr<UJobViewModel>> JobViewModels;
	
	void LoadJobs();
	/** Creates ViewModels for each loaded Job */
	void OnJobsLoaded();

	void CreateJobViewModel(UJobDefinition* JobDefinition);

	UFUNCTION()
	void OnMainJobChanged();
	
	UFUNCTION()
	void OnSubJobChanged();

	UFUNCTION()
	void OnTrySetJob(bool bSuccess);

	UFUNCTION()
	void OnJobProgressUpdated(const FJobProgressItem& JobProgressItem);
};
