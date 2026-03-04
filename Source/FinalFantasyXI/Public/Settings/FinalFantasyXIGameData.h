// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FinalFantasyXIGameData.generated.h"

class UCrysGameplayTagRelationship;
class UAttributeTagRelationship;
class UGameplayEffect;

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, DisplayName = "Final Fantasy XI Game Data")
class FINALFANTASYXI_API UFinalFantasyXIGameData : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	// Maps Gameplay tags to attributes. Used in the UI and for Gameplay Effects to set attributes.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TSoftObjectPtr<UAttributeTagRelationship> AttributeTagRelationship;

	// Gameplay effect used to add and remove dynamic tags.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem|Gameplay Effect")
	TSoftClassPtr<UGameplayEffect> DynamicTagGameplayEffect;
	
	// Maps non attribute GameplayTags for UI user facing data.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UCrysGameplayTagRelationship> CrysGameplayTagRelationship;
};
