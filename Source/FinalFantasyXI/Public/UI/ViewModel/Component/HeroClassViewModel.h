// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MVVMViewModelBase.h"
#include "HeroSystem/HeroTypes.h"
#include "HeroClassViewModel.generated.h"

class UHeroClassDefinition;

/**
 * Information about the HeroClass and it's current progress. The data is managed by the HeroManagerViewModel.
 */
UCLASS()
class FINALFANTASYXI_API UHeroClassViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	bool IsHeroClassAvailable() const { return bHeroClassAvailable; }
	bool IsCurrentHeroClass() const { return bCurrentHeroClass; }

	UFUNCTION(BlueprintPure, FieldNotify, Category = "HeroClass ViewModel")
	FText GetHeroClassName() const;

	UFUNCTION(BlueprintPure, FieldNotify, Category = "HeroClass ViewModel")
	FText GetShortName() const;

	UFUNCTION(BlueprintPure, FieldNotify, Category = "HeroClass ViewModel")
	FText GetDescription() const;

	UFUNCTION(BlueprintPure, FieldNotify, Category = "HeroClass ViewModel")
	int32 GetLevel() const { return HeroClassProgressItem.Level; }
	UFUNCTION(BlueprintPure, FieldNotify, Category = "HeroClass ViewModel")
	int32 GetExperience() const { return HeroClassProgressItem.Experience; }
	UFUNCTION(BlueprintPure, FieldNotify, Category = "HeroClass ViewModel")
	int32 GetExperienceRequiredUntilNextLevel() const;
	UFUNCTION(BlueprintPure, FieldNotify, Category = "HeroClass ViewModel")
	float GetPercentageTowardsNextLevel() const;

	void SetHeroClassAndProgress(UHeroClassDefinition* InHeroClass, const FHeroClassProgressItem& InHeroClassProgressItem);
	void SetHeroClassProgressItem(const FHeroClassProgressItem& InHeroClassProgressItem);
	void SetIsCurrentHeroClass(bool InValue);

	UHeroClassDefinition* GetHeroClass() const { return HeroClassDefinition; }

protected:
	void SetIsHeroClassAvailable(bool InValue);
	void SetHeroClassDefinition(UHeroClassDefinition* InHeroClass);

private:
	/** The cached value of the HeroClassDefinition. */
	UPROPERTY()
	TObjectPtr<UHeroClassDefinition> HeroClassDefinition;

	/** Cached value of the ProgressItem. */
	UPROPERTY()
	FHeroClassProgressItem HeroClassProgressItem;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter="IsHeroClassAvailable", Category = "HeroClass ViewModel", meta=(AllowPrivateAccess=true))
	bool bHeroClassAvailable = false;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter="IsCurrentHeroClass", Category = "HeroClass ViewModel", meta=(AllowPrivateAccess=true))
	bool bCurrentHeroClass = false;
};
