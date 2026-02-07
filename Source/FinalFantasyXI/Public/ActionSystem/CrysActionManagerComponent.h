// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CrysActionManagerComponent.generated.h"


class ACrysPlayerController;
class UCrysAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCrysActionManagerActionUpdateSignature, UCrysAction*, Action, const FGameplayTag&, InputTag, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrysActionManagerActionSetSignature, int32, Index);

/**
 * A class that manages the actions that are activatable by InputTag. This component is designed to only live on the CrysPlayerController.
 * This is like a Hotbar, ActionBar, QuickSlot functionality.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINALFANTASYXI_API UCrysActionManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCrysActionManagerComponent();
	virtual void OnRegister() override;
	
	/** Called whenever an action map is changed/added/removed. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnActionMapUpdated")
	FCrysActionManagerActionUpdateSignature OnActionMapUpdatedDelegate;
	
	/** Called when the chosen action set has changed. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnActionSetSelected")
	FCrysActionManagerActionSetSignature OnActionSetSelectedDelegate;
	
	UFUNCTION(BlueprintPure, Category = "CrysActionManager")
	UCrysAction* FindAction(UPARAM(meta = (Categories="Input")) FGameplayTag InputTag, int32 Index) const;
	
	UFUNCTION(BlueprintPure, Category = "CrysActionManager")
	UCrysAction* FindActionByClass(const TSubclassOf<UCrysAction>& ActionClass) const;
	
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	bool CreateActionAndTryActivateOnce(const TSubclassOf<UCrysAction>& ActionClass);
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	bool TryActivateAction(UPARAM(meta = (Categories="Input")) FGameplayTag InputTag);
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	bool TryActivateActionAtIndex(UPARAM(meta = (Categories="Input")) FGameplayTag InputTag, int32 Index);
	
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	void SetAction(UPARAM(meta = (Categories = "Input")) const FGameplayTag& InputTag, const int32 Index, const TSubclassOf<UCrysAction>& ActionClass);
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	void ClearAction(UPARAM(meta = (Categories = "Input")) FGameplayTag InputTag, int32 Index);
	
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	void SetCurrentActionSetIndex(int32 Index);
	UFUNCTION(BlueprintPure, Category = "CrysActionManager")
	int32 GetCurrentActionSetIndex() const { return CurrentActionSetIndex; }
	
	//SetActionMap
	//SetActionMappings
	//ClearActionMap
	//ClearActionMappings
	
private:
	/** 
	 * The active mappings. 
	 * The Array Index is an "ActionSet" and the TMap is the InputTag to Action Map.
	 */
	TArray<TMap<FGameplayTag, TObjectPtr<UCrysAction>>> ActionMappings;
	
	/** A cache of actions that were created to be activated once. */
	UPROPERTY()
	TArray<TObjectPtr<UCrysAction>> ActionPool;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1))
	int32 MaxCacheSize = 10;
	
	/** As items are added to the ActionPool cache. Increment by 1, then reset once we reach max cache size. */
	int32 CurrentCacheIndex = 0;
	
	int32 CurrentActionSetIndex = 0;
	
	UPROPERTY()
	TObjectPtr<ACrysPlayerController> PlayerController;
	
	// CreateActionInstance
	UCrysAction* CreateActionInstance(const TSubclassOf<UCrysAction>& ActionClass);
};

