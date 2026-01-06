// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EquipmentSystemBlueprintFunctionLibrary.generated.h"

class UEquipmentManagerComponent;
/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UEquipmentSystemBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Gets the EquipmentManagerComponent, checking the EquipmentSystemInterface and then falling back to searching the
	 * Actor's components.
	 */
	UFUNCTION(BlueprintPure, Category = "EquipmentSystem", meta = (DefaultToSelf = "Actor"))
	static UEquipmentManagerComponent* GetEquipmentManagerComponent(AActor* Actor);
};
