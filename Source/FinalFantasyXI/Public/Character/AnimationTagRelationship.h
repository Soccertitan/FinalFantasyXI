// Copyright 2025 Soccertitan

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AnimationTagRelationship.generated.h"

USTRUCT(BlueprintType)
struct FAnimationRelationshipItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AnimationTag;
	
	// The montage to play.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> Montage;
	
	// The delay to wait when invoking the montage to trigger the attack.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0, Units = s))
	float AttackDelay = 0.f;
	
	void PostSerialize(const FArchive& Ar);
	
	bool IsValid() const;
	
#if WITH_EDITORONLY_DATA
	/** Hidden used as a TitleProperty in the AnimationRelationshipItem. */
	UPROPERTY(VisibleDefaultsOnly, meta = (EditCondition=false, EditConditionHides))
	FString EditorDisplayName;
#endif

	friend bool operator==(const FAnimationRelationshipItem& X, const FAnimationRelationshipItem& Y)
	{
		return X.AnimationTag == Y.AnimationTag;
	}
	friend bool operator!=(const FAnimationRelationshipItem& X, const FAnimationRelationshipItem& Y)
	{
		return X.AnimationTag != Y.AnimationTag;
	}
};
template<>
struct TStructOpsTypeTraits<FAnimationRelationshipItem> : public TStructOpsTypeTraitsBase2<FAnimationRelationshipItem>
{
	enum
	{
		WithPostSerialize = true,
   };
};

/**
 * Links an tag to animation data.
 */
UCLASS(Const)
class FINALFANTASYXI_API UAnimationTagRelationship : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UAnimationTagRelationship();
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Primary Asset")
	FPrimaryAssetType AssetType;

	UFUNCTION(BlueprintPure)
	FAnimationRelationshipItem FindAnimationRelationshipItem(UPARAM(ref) const FGameplayTag& AnimationTag, bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (NoResetToDefault, ForceInlineRow, TitleProperty = "{EditorDisplayName}"))
	TArray<FAnimationRelationshipItem> AnimationRelationshipItems;
};
