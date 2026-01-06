// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeTagRelationship.generated.h"

/** Defines the relationship between a GameplayTag and attribute information. */
USTRUCT(BlueprintType)
struct FAttributeRelationshipItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (Categories = "Attribute"))
	FGameplayTag AttributeTag;

	/** The normal name of the attribute. */
	UPROPERTY(EditAnywhere)
	FText Name;
	
	/** The short name of the attribute. */
	UPROPERTY(EditAnywhere)
	FText ShortName;

	/** Description of the attribute. */
	UPROPERTY(EditAnywhere)
	FText Description;

	/** Icon for this attribute. */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Icon;

	/** The attribute to query from the ASC. */
	UPROPERTY(EditAnywhere)
	FGameplayAttribute GameplayAttribute;

	/** Evaluates the GameplayAttribute with source tags. */
	UPROPERTY(EditAnywhere)
	FGameplayTagRequirements SourceTags;

	bool IsValid() const;

	friend bool operator==(const FAttributeRelationshipItem& X, const FAttributeRelationshipItem& Y)
	{
		return X.AttributeTag == Y.AttributeTag;
	}
	friend bool operator!=(const FAttributeRelationshipItem& X, const FAttributeRelationshipItem& Y)
	{
		return X.AttributeTag != Y.AttributeTag;
	}
};

/**
 * Maps data about an Attribute gameplay tag to its properties for UI purposes.
 */
UCLASS(Const)
class FINALFANTASYXI_API UAttributeTagRelationship : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UAttributeTagRelationship();
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Primary Asset")
	FPrimaryAssetType AssetType;

	UFUNCTION(BlueprintPure)
	FAttributeRelationshipItem FindAttributeRelationshipItem(UPARAM(ref) const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Attribute", NoResetToDefault, ForceInlineRow, TitleProperty = "{AttributeTag}"))
	TArray<FAttributeRelationshipItem> AttributeTagRelationships;
};
