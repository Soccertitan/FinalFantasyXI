// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "HeroSystem/HeroTypes.h"
#include "HeroJobViewModel.generated.h"

class UHeroJobDefinition;

/**
 * Information about the HeroJob and it's current progress. The data is managed by the HeroManagerViewModel.
 */
UCLASS()
class FINALFANTASYXI_API UHeroJobViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	bool IsHeroJobAvailable() const { return bHeroJobAvailable; }
	bool IsMainJob() const { return bMainJob; }
	bool IsSubJob() const { return bSubJob; }

	UFUNCTION(BlueprintPure, FieldNotify, Category = "HeroJob ViewModel")
	FText GetHeroJobName() const;

	UFUNCTION(BlueprintPure, FieldNotify, Category = "HeroJob ViewModel")
	FText GetShortName() const;

	UFUNCTION(BlueprintPure, FieldNotify, Category = "HeroJob ViewModel")
	FText GetDescription() const;

	UFUNCTION(BlueprintPure, FieldNotify, Category = "HeroJob ViewModel")
	int32 GetLevel() const { return HeroJobProgressItem.Level; }
	UFUNCTION(BlueprintPure, FieldNotify, Category = "HeroJob ViewModel")
	int32 GetExperience() const { return HeroJobProgressItem.Experience; }
	UFUNCTION(BlueprintPure, FieldNotify, Category = "HeroJob ViewModel")
	int32 GetExperienceRequiredUntilNextLevel() const;
	UFUNCTION(BlueprintPure, FieldNotify, Category = "HeroJob ViewModel")
	float GetPercentageTowardsNextLevel() const;

	void SetHeroJobAndProgress(UHeroJobDefinition* InHeroJob, const FHeroJobProgressItem& InHeroJobProgressItem);
	void SetHeroJobProgressItem(const FHeroJobProgressItem& InHeroJobProgressItem);
	void SetIsMainJob(bool InValue);
	void SetIsSubJob(bool InValue);

	UHeroJobDefinition* GetHeroJob() const { return HeroJobDefinition; }

protected:
	void SetIsHeroJobAvailable(bool InValue);
	void SetHeroJobDefinition(UHeroJobDefinition* InHeroJob);

private:
	/** The cached value of the HeroJobDefinition. */
	UPROPERTY()
	TObjectPtr<UHeroJobDefinition> HeroJobDefinition;

	/** Cached value of the ProgressItem. */
	UPROPERTY()
	FHeroJobProgressItem HeroJobProgressItem;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter="IsHeroJobAvailable", Category = "HeroJob ViewModel", meta=(AllowPrivateAccess=true))
	bool bHeroJobAvailable = false;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter="IsMainJob", Category = "HeroJob ViewModel", meta=(AllowPrivateAccess=true))
	bool bMainJob = false;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter="IsSubJob", Category = "HeroJob ViewModel", meta=(AllowPrivateAccess=true))
	bool bSubJob = false;
};
