// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HeroSystemInterface.generated.h"

class UHeroManagerComponent;

// This class does not need to be modified.
UINTERFACE()
class UHeroSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FINALFANTASYXI_API IHeroSystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	UHeroManagerComponent* GetHeroManagerComponent() const;
};
