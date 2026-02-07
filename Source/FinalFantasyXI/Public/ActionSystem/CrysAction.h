// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CrysAction.generated.h"

class UCrysActionManagerComponent;
class UCrysAction;
class ACrysPlayerController;

/**
 * Defines an action that can be mapped to an ActionManager or activated on its own.
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class FINALFANTASYXI_API UCrysAction : public UObject
{
	GENERATED_BODY()
	
public:
	UCrysAction();
	
	/** The name of the action. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ActionName;
	
	/** The icon to display on the ActionBar. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AssetBundles = "UI"))
	TSoftObjectPtr<UTexture2D> Icon;
	
	UFUNCTION(BlueprintCallable, Category = "CrysAction")
	bool TryActivateAction();
	
	virtual bool CanActivateAction() const;
	
protected:
	virtual void InitializeAction() {}

	virtual void ActivateAction() {}
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "ActivateAction")
	void K2_ActivateAction();
	
	UFUNCTION(BlueprintNativeEvent, DisplayName = "CanActivateAction")
	bool K2_CanActivateAction() const;
	
	UFUNCTION(BlueprintPure)
	ACrysPlayerController* GetPlayerController() const { return PlayerController; }
	
private:
	UPROPERTY()
	TObjectPtr<ACrysPlayerController> PlayerController;
	
	friend UCrysActionManagerComponent;
};
