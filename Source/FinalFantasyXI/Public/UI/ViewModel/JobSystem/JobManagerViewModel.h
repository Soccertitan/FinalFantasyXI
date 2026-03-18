// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/StreamableManager.h"
#include "JobSystem/JobTypes.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "Types/MVVMEventField.h"
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
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	FMVVMEventField JobsLoaded() const { return {}; }
	
	bool GetIsSwitchingJobs() const { return bSwitchingJobs; }

	UJobViewModel* GetMainJobViewModel() const { return MainJobViewModel; }
	UJobViewModel* GetSubJobViewModel() const { return SubJobViewModel; }
	
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	bool IsSubJobEquipped() const;
	
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	int32 GetLevel() const { return JobManagerData.Level; }
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	int32 GetExperience() const { return JobManagerData.Experience; }
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	int32 GetMaxLevel() const { return JobManagerData.MaxLevel; }
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	int32 GetMaxJobLevel() const { return JobManagerData.MaxJobLevel; }
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	bool IsSubJobUnlocked() const { return JobManagerData.bSubJobUnlocked; }
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	float GetSubJobEfficiency() const { return JobManagerData.SubJobEfficiency; } 

	/** Finds a ViewModel with the specified JobTag. */
	UFUNCTION(BlueprintPure, Category = "Viewmodel|Job")
	UJobViewModel* FindJobViewModel(UPARAM(meta = (Categories = "Job")) FGameplayTag JobTag);

	/** Tries to switch to specified Job. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Job")
	void TrySetJobs(UJobViewModel* InMainJobViewModel, UJobViewModel* InSubJobViewModel);
	
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Job")
	void TrySetMainJob(UJobViewModel* JobViewModel);
	
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Job")
	void TrySetSubJob(UJobViewModel* JobViewModel);

protected:
	virtual void OnInitializeViewModel(APlayerController* PlayerController) override;

	void SetMainJobViewModel(UJobViewModel* InValue);
	void SetSubJobViewModel(UJobViewModel* InValue);

private:
	/** The JobDefinition to load. */
	UPROPERTY(EditAnywhere, Category = "JobManagerViewModel", meta = (AllowedTypes = "JobDefinition"))
	TArray<FPrimaryAssetId> JobsToLoad;

	/** Cached pointer to the JobManagerComponent from the PlayerState. */
	UPROPERTY()
	TObjectPtr<UJobManagerComponent> JobManagerComponent;
	
	UPROPERTY()
	FJobManagerData JobManagerData;

	/** True if waiting to switch Jobs. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter = "GetIsSwitchingJobs", meta = (AllowPrivateAccess = "true"))
	bool bSwitchingJobs = false;

	/** Cached handle for the JobDefinition. */
	TSharedPtr<FStreamableHandle> JobStreamableHandle;

	/** The current MainJob the player is. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UJobViewModel> MainJobViewModel;
	
	/** The current SubJob the player is. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, meta = (AllowPrivateAccess = "true"))
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
	
	UFUNCTION()
	void OnJobManagerDataUpdated();
};
