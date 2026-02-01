// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputActionListenerMap.generated.h"

class UInputAction;
class UInputActionListener;

/**
 * A map of InputActions to InputActionListeners.
 */
UCLASS()
class FINALFANTASYXI_API UInputActionListenerMap : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (NoResetToDefault))
	TMap<TObjectPtr<UInputAction>, TSubclassOf<UInputActionListener>> InputActionMap;
};
