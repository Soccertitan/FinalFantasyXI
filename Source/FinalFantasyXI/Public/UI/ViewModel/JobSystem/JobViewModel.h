// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "JobSystem/JobTypes.h"
#include "JobViewModel.generated.h"

class UJobDefinition;

/**
 * Information about the HeroJob and it's current progress. The data is managed by the HeroManagerViewModel.
 */
UCLASS()
class FINALFANTASYXI_API UJobViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	bool IsJobAvailable() const { return bJobAvailable; }
	bool IsMainJob() const { return bMainJob; }
	bool IsSubJob() const { return bSubJob; }

	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	FText GetJobName() const;

	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	FText GetShortName() const;

	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	FText GetDescription() const;

	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	int32 GetLevel() const { return JobProgressItem.Level; }
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	int32 GetExperience() const { return JobProgressItem.Experience; }
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	int32 GetExperienceRequiredUntilNextLevel() const;
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	float GetPercentageTowardsNextLevel() const;

	void SetJobAndProgress(UJobDefinition* InHeroJob, const FJobProgressItem& InHeroJobProgressItem);
	void SetJobProgressItem(const FJobProgressItem& InHeroJobProgressItem);
	void SetIsMainJob(bool InValue);
	void SetIsSubJob(bool InValue);

	UJobDefinition* GetJob() const { return JobDefinition; }

protected:
	void SetIsJobAvailable(bool InValue);
	void SetJobDefinition(UJobDefinition* InHeroJob);

private:
	/** The cached value of the HeroJobDefinition. */
	UPROPERTY()
	TObjectPtr<UJobDefinition> JobDefinition;

	/** Cached value of the ProgressItem. */
	UPROPERTY()
	FJobProgressItem JobProgressItem;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter="IsJobAvailable", meta=(AllowPrivateAccess=true))
	bool bJobAvailable = false;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter="IsMainJob", meta=(AllowPrivateAccess=true))
	bool bMainJob = false;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter="IsSubJob", meta=(AllowPrivateAccess=true))
	bool bSubJob = false;
};
