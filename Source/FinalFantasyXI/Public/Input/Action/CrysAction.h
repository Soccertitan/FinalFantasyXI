// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrimAction.h"
#include "CrysAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrysActionGenericSignature, UCrysAction*, Action);

/**
 * A generic action that broadcasts when an input is pressed/released.
 */
UCLASS()
class FINALFANTASYXI_API UCrysAction : public UCrimAction
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FCrysActionGenericSignature OnInputPressed;
	UPROPERTY(BlueprintAssignable)
	FCrysActionGenericSignature OnInputReleased;

protected:
	virtual void OnInputActionTriggered(const FInputActionValue& Value) override;
	virtual void OnInputActionCanceled(const FInputActionValue& Value) override;
	virtual void OnInputActionCompleted(const FInputActionValue& Value) override;
};
