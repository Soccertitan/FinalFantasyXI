// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CrysGameplayTagRelationship.generated.h"

/** Defines the relationship between a GameplayTag and UI information. */
USTRUCT()
struct FCrysGameplayTagRelationshipItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;
	
	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText ShortName;
	
	UPROPERTY(EditAnywhere, meta = (MultiLine=true))
	FText Description;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> Icon;
	
	friend bool operator==(const FCrysGameplayTagRelationshipItem& X, const FCrysGameplayTagRelationshipItem& Y)
	{
		return X.Tag == Y.Tag;
	}
	friend bool operator!=(const FCrysGameplayTagRelationshipItem& X, const FCrysGameplayTagRelationshipItem& Y)
	{
		return X.Tag != Y.Tag;
	}
};

/**
 * Data about a GameplayTag and it's information.
 */
UCLASS(Const)
class FINALFANTASYXI_API UCrysGameplayTagRelationship : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	FCrysGameplayTagRelationshipItem FindAttributeRelationshipItem(UPARAM(ref) const FGameplayTag& Tag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, meta = (NoResetToDefault, ForceInlineRow, TitleProperty = "{Tag}"))
	TArray<FCrysGameplayTagRelationshipItem> CrysGameplayTagRelationshipItems;
	
	static FCrysGameplayTagRelationshipItem FindCrysGameplayTagRelationshipItem(const FGameplayTag& Tag, bool bLogNotFound = false);
};
