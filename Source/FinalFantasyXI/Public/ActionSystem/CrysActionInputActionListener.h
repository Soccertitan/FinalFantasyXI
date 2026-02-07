// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Input/InputAction/CrysInputActionListener.h"
#include "CrysActionInputActionListener.generated.h"

struct FGameplayTag;
class UCrysActionManagerComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FCrysActionActionSetIndexSignature, int32);

/**
 * Activates the action in the currently selected index of the ActionManager.
 */
UCLASS(Abstract)
class FINALFANTASYXI_API UCrysActionInputActionListener : public UCrysInputActionListener
{
	GENERATED_BODY()
	
public:

protected:
	virtual void OnInitializeListener() override;
	
	virtual void OnInputActionTriggered(const FInputActionValue& Value) override;
	
private:
	/** The action "slot" to activate on the ActionBar. */
	UPROPERTY(EditAnywhere, meta = (Categories="Input"))
	FGameplayTag InputTag;
	
	UPROPERTY()
	TObjectPtr<UCrysActionManagerComponent> ActionManager;
};
