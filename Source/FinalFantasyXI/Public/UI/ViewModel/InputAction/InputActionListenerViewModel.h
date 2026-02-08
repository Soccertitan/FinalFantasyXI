// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "InputActionListenerViewModel.generated.h"

class UActionManagerViewModel;
class UCrysInputActionListener;
class UInputAction;

/**
 * A generic ViewModel that broadcasts when the IA is pressed/released.
 */
UCLASS()
class FINALFANTASYXI_API UInputActionListenerViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	void InitializeInputActionListener(UInputAction* InInputAction, UCrysInputActionListener* InInputActionListener);
	UInputAction* GetInputAction() const { return InputAction; }
	UCrysInputActionListener* GetInputActionListener() const {return InputActionListener;}
	
	bool IsPressed() const { return bPressed; }
	
protected:
	void SetIsPressed(bool InValue);
	
private:
	UPROPERTY()
	TObjectPtr<UInputAction> InputAction;
	UPROPERTY()
	TObjectPtr<UCrysInputActionListener> InputActionListener;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter=IsPressed, meta = (AllowPrivateAccess = true))
	bool bPressed = false;
	
	UFUNCTION()
	void OnInputPressed(UCrysInputActionListener* Action);
	UFUNCTION()
	void OnInputReleased(UCrysInputActionListener* Action);
};
