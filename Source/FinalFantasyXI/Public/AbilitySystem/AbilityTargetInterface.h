// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilityTargetInterface.generated.h"

struct FGameplayTagContainer;

// This class does not need to be modified.
UINTERFACE()
class UAbilityTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Called from within an Ability when an Actor is required to commit the ability.
 */
class FINALFANTASYXI_API IAbilityTargetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	AActor* GetAbilityTarget(const FGameplayTagContainer& ContextTags) const;
};
