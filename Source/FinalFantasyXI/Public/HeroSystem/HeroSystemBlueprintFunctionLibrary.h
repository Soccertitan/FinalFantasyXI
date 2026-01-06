// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HeroSystemBlueprintFunctionLibrary.generated.h"

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
};
