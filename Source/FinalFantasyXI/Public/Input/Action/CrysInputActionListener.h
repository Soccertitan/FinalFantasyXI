// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "InputActionListener.h"
#include "CrysInputActionListener.generated.h"

class ACrysPlayerController;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrysInputActionListenerGenericSignature, UCrysInputActionListener*, Action)
;

/**
 * A generic action that broadcasts when an input is pressed/released.
 */
UCLASS()
class FINALFANTASYXI_API UCrysInputActionListener : public UInputActionListener
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FCrysInputActionListenerGenericSignature OnInputPressed;
	UPROPERTY(BlueprintAssignable)
	FCrysInputActionListenerGenericSignature OnInputReleased;

protected:
	virtual void OnInputActionTriggered(const FInputActionValue& Value) override;
	virtual void OnInputActionCanceled(const FInputActionValue& Value) override;
	virtual void OnInputActionCompleted(const FInputActionValue& Value) override;
	
private:
	UPROPERTY()
	bool bPressed = false;
};
