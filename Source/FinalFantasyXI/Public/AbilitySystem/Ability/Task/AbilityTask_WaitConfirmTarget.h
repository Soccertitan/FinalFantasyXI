// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitConfirmTarget.generated.h"

class UCameraComponent;
struct FInputActionValue;
class UTargetPointComponent;
class UWidgetComponent;
enum class EAbilityTargetType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitConfirmTargetSignature, AActor*, TargetedActor);

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UAbilityTask_WaitConfirmTarget : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	/**
	 * Starts a targeting routine to find a TargetPointComponent for the local player.
	 * @param InitialTarget The initial target to try and set to.
	 * @param TargetType Whom to allow targeting.
	 * @param TargetWidgetClass The widget class to visually represent the currently targeted actor.
	 * @param TargetWidgetParentSocket The socket to attach the widget to.
	 * @param TargetingRange The range of the targeting choices.
	 * @param CheckFrequency The frequency to check for line of sight breaks.
	 * @param BreakTargetingDelay The delay when line of sight is broken before cancelling the targeting task.
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "Wait Confirm Target", Category = "Ability|Tasks", meta =(HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", 
		AdvancedDisplay = "CheckFrequency, BreakTargetingDelay"))
	static UAbilityTask_WaitConfirmTarget* WaitConfirmTarget(
		UGameplayAbility* OwningAbility,
		AActor* InitialTarget,
		EAbilityTargetType TargetType,
		TSubclassOf<UUserWidget> TargetWidgetClass,
		FName TargetWidgetParentSocket = FName("Target"),
		float TargetingRange = 1500.f,
		float CheckFrequency = 0.1f,
		float BreakTargetingDelay = 2.0f);
	
protected:
	UPROPERTY()
	TObjectPtr<AActor> CurrentTarget;
	UPROPERTY()
	EAbilityTargetType TargetType;
	UPROPERTY()
	TSubclassOf<UUserWidget> TargetWidgetClass;
	UPROPERTY()
	FName TargetWidgetParentSocket;
	float TargetingRange;
	/** Frequency to check if the target is in line of sight, within range, and is generally targetable. */
	float CheckFrequency;
	/** The amount of time to break targeting when the Actor is too far away or obstructed behind an Object. */
	float BreakTargetingDelay;
	
public:
	UPROPERTY(BlueprintAssignable)
	FWaitConfirmTargetSignature OnConfirm;
	UPROPERTY(BlueprintAssignable)
	FWaitConfirmTargetSignature OnCancel;
	
	virtual void OnDestroy(bool bInOwnerFinished) override;
	
protected:
	UPROPERTY()
	TObjectPtr<UWidgetComponent> TargetWidgetComponent;
	UPROPERTY()
	TObjectPtr<UTargetPointComponent> CurrentTargetPoint;
	
	bool RegisteredCallbacks = false;
	
	virtual void Activate() override;
	
	void SetTarget(UTargetPointComponent* TargetPoint);
	UTargetPointComponent* FindNearestTarget() const;
	UTargetPointComponent* FindNextTarget(UTargetPointComponent* OriginPoint, bool bSearchLeft = false) const;
	
	void GetTargetablePoints(TArray<UTargetPointComponent*>& OutResults) const;
	
	void IA_CycleTarget(const FInputActionValue& Value);
	
private:
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UFUNCTION()
	void OnReplicatedConfirmCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
	UFUNCTION()
	void OnReplicatedCancelCallback();
	UFUNCTION()
	void OnLocalConfirmCallback();
	UFUNCTION()
	void OnLocalCancelCallback();
	
	void BindToInputActions();
	void RemoveInputActions();
	
	/** Attaches a widget to the component. */
	void CreateAndAttachTargetSelectedWidgetComponent(UTargetPointComponent* InTargetPoint);
	
	void SetupInitialTarget();
	
	/**
	 * Functionality to clear the target if line of sight is broken during target selection
	 */
	UPROPERTY()
	FTimerHandle CheckTargetPointTimerHandle;
	UPROPERTY()
	FTimerHandle BreakTargetPointTimerHandle;
	UPROPERTY()
	bool bIsBreakingLineOfSight;
	
	void CheckTargetPoint();
	bool ShouldBreakTargeting() const;
	void BreakTargeting();
	
	float GetDistanceToPoint(const UTargetPointComponent* InTargetPoint) const;
};
