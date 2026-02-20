// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CombatAnimationData.generated.h"

/**
 * Combat montage information.
 */
UCLASS(Const)
class FINALFANTASYXI_API UCombatAnimationData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UCombatAnimationData();
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Primary Asset")
	FPrimaryAssetType AssetType;
	
	/** Montages of the attacks when standing still. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (NoResetToDefault, AssetBundles = "Animation"))
	TArray<TSoftObjectPtr<UAnimMontage>> AutoAttacks;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (NoResetToDefault, AssetBundles = "Animation"))
	TSoftObjectPtr<UAnimMontage> EnterCombat;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (NoResetToDefault, AssetBundles = "Animation"))
	TSoftObjectPtr<UAnimMontage> ExitCombat;
};
