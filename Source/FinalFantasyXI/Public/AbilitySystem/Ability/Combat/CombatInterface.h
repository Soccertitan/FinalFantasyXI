// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CombatTypes.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FINALFANTASYXI_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FWeaponData GetPrimaryWeaponData() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FWeaponData GetSecondaryWeaponData() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetTargetActor();
};
