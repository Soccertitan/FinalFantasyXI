// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HeroSystemBlueprintFunctionLibrary.generated.h"

struct FLevelProgressContainer;
struct FScalableFloat;
class UHeroManagerComponent;
/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UHeroSystemBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Gets the HeroManagerComponent, checking the HeroSystemInterface and then falling back to searching the Actor's
	 * components.
	 */
	UFUNCTION(BlueprintPure, Category = "HeroSystem", meta = (DefaultToSelf = "Actor"))
	static UHeroManagerComponent* GetHeroManagerComponent(const AActor* Actor);
	
	/** 
	 * Adds experience and adjust the level according to the experience requirement.
	 * @param InLevel The level to update.
	 * @param InExperience The experience to update.
	 * @param ExperienceToGrant The amount of experience to grant.
	 * @param ExperienceRequirement A curve table with increasing values as the level increases.
	 * @param MaxLevel If less than 0, there is no max level and will grow until Max_Int32.
	 * @return True, if the Level was adjusted.
	 */
	UFUNCTION(BlueprintCallable, Category = "HeroSystem")
	static bool AddExperience(int32& InLevel, int32& InExperience, const int32 ExperienceToGrant, const FScalableFloat& ExperienceRequirement, int32 MaxLevel = -1);
	
	/** 
	 * Subtracts experience from the container and adjust the level according to the experience requirement.
	 * @param InLevel The level to update.
	 * @param InExperience The experience to update.
	 * @param ExperienceToRemove The amount of experience to subtract. Must be a positive number.
	 * @param ExperienceRequirement A curve table with increasing values as the level increases.
	 * @return True, if the Level was adjusted.
	 */
	UFUNCTION(BlueprintCallable, Category = "HeroSystem")
	static bool SubtractExperience(int32& InLevel, int32& InExperience, const int32 ExperienceToRemove, const FScalableFloat& ExperienceRequirement);
	
	/** 
	 * Sets the level for the container and adjust the experience according to the experience requirement.
	 * @param InLevel The level to update.
	 * @param InExperience The experience to update.
	 * @param NewLevel The level to set the container to.
	 * @param ExperienceRequirement A curve table with increasing values as the level increases.
	 */
	UFUNCTION(BlueprintCallable, Category = "HeroSystem")
	static void SetLevel(int32& InLevel, int32& InExperience, const int32& NewLevel, const FScalableFloat& ExperienceRequirement);
};
