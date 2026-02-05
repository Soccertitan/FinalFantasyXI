// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityAnimationData.generated.h"

/**
 * Ability and casting montages mapped to gameplay tags.
 */
UCLASS(Const)
class FINALFANTASYXI_API UAbilityAnimationData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UAbilityAnimationData();
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Primary Asset")
	FPrimaryAssetType AssetType;
	
	/** If a tag does not exist in the map, The first montage will be used. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (NoResetToDefault, ForceInlineRow))
	TMap<FGameplayTag, TSoftObjectPtr<UAnimMontage>> CastingMap;
	
	/** If a tag does not exist in the map, The first montage will be used. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (NoResetToDefault, ForceInlineRow))
	TMap<FGameplayTag, TSoftObjectPtr<UAnimMontage>> AbilityMap;
	
	/** Returns the animation data for a casting ability. If none found, gets the first value in the map. */
	UFUNCTION(BlueprintPure, Category = "AbilityAnimationData")
	TSoftObjectPtr<UAnimMontage> GetCastingMontage(const FGameplayTag& Tag);
	
	/** Returns the animation data for an ability. If none found, gets the first value in the map. */
	UFUNCTION(BlueprintPure, Category = "AbilityAnimationData")
	TSoftObjectPtr<UAnimMontage> GetAbilityMontage(const FGameplayTag& Tag);
};
