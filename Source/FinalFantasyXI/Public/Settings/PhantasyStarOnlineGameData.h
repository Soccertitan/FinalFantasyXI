// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PhantasyStarOnlineGameData.generated.h"

class UAttributeTagRelationship;
class UGameplayEffect;
/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, DisplayName = "Phantasy Star Online Game Data")
class FINALFANTASYXI_API UPhantasyStarOnlineGameData : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	// Maps Gameplay tags to attributes. Used in the UI and for Gameplay Effects to set attributes.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TSoftObjectPtr<UAttributeTagRelationship> AttributeTagRelationship;
	
	// Gameplay effect used to add and remove dynamic tags.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem|Gameplay Effect")
	TSoftClassPtr<UGameplayEffect> DynamicTagGameplayEffect;

	// Generic Gameplay effect for applying equipment stats.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem|Gameplay Effect")
	TSoftClassPtr<UGameplayEffect> EquipmentGameplayEffect;
};
