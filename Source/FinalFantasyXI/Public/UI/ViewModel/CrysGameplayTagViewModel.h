// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "UI/CrysGameplayTagRelationship.h"
#include "CrysGameplayTagViewModel.generated.h"

struct FGameplayTag;

/**
 * Display information on CrysGameplayTags
 */
UCLASS()
class FINALFANTASYXI_API UCrysGameplayTagViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	void SetGameplayTag(const FGameplayTag& InValue);
	
	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetTagName() const {return RelationshipItem.Name;}
	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetShortName() const {return RelationshipItem.ShortName;}
	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetDescription() const {return RelationshipItem.Description;}
	UFUNCTION(BlueprintPure, FieldNotify)
	TSoftObjectPtr<UTexture2D> GetIcon() const {return RelationshipItem.Icon;}
	
private:
	UPROPERTY()
	FCrysGameplayTagRelationshipItem RelationshipItem;
};
