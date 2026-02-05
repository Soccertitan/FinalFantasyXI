// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AutoAttackAnimationData.generated.h"

/**
 * AutoAttack montage information.
 */
UCLASS(Const)
class FINALFANTASYXI_API UAutoAttackAnimationData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UAutoAttackAnimationData();
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Primary Asset")
	FPrimaryAssetType AssetType;
	
	/** Montages of the attacks when standing still. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (NoResetToDefault, AssetBundles = "Animation"))
	TArray<TSoftObjectPtr<UAnimMontage>> AutoAttacks;
};
