// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrysAction.h"
#include "EnhancedInputSubsystemInterface.h"
#include "ActionInputMappingContext.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputMappingContext;

/**
 * Adds/Removes an Input Mapping Context when Triggered/Completed/Canceled.
 */
UCLASS(Abstract)
class FINALFANTASYXI_API UActionInputMappingContext : public UCrysAction
{
	GENERATED_BODY()
	
protected:
	virtual void OnInitializeAction() override;

	virtual void OnInputActionTriggered(const FInputActionValue& Value) override;
	virtual void OnInputActionCompleted(const FInputActionValue& Value) override;
	virtual void OnInputActionCanceled(const FInputActionValue& Value) override;
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY(EditAnywhere)
	int32 Priority = 1;
	UPROPERTY(EditAnywhere)
	FModifyContextOptions ContextOptions;
	
	UPROPERTY()
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> EnhancedInputSubsystem;
	
	bool bAppliedContext = false;
};
