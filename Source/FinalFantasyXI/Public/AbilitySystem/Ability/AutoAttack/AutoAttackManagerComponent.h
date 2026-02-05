// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrimAbilitySystemInterface.h"
#include "Components/ActorComponent.h"
#include "AutoAttackManagerComponent.generated.h"

class UAutoAttackAnimationData;
struct FGameplayTag;
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAutoAttackManagerBoolSignature, bool, bAutoAttacking);
DECLARE_MULTICAST_DELEGATE_OneParam(FAutoAttackManagerAnimationDataSignature, UAutoAttackAnimationData*);

/**
 * Manages the timer for auto attacks. Notifies when the timer starts/ends. And updates timers based on specified 
 * conditions. Like equipping a new weapon.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINALFANTASYXI_API UAutoAttackManagerComponent : public UActorComponent, public ICrimAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UAutoAttackManagerComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void PreNetReceive() override;
	
	virtual void InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent) override;
	
	UPROPERTY(BlueprintAssignable, DisplayName = "OnAutoAttackStateChanged")
	FAutoAttackManagerBoolSignature OnAutoAttackStateChangedDelegate;
	
	/** Starts the auto attack timer. */
	UFUNCTION(BlueprintCallable, Category = "AutoAttackManager")
	void StartAutoAttack();

	/** Ends the auto attack timer. */
	UFUNCTION(BlueprintCallable, Category = "AutoAttackManager")
	void StopAutoAttack();
	
	/** If AutoAttacking, clears the timer handle and starts a new one. */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AutoAttackManager")
	void RestartAutoAttackTimer();
	
	UFUNCTION(BlueprintPure, Category = "AutoAttackManager")
	bool IsAutoAttacking() const;
	
	/** Called when the PrimaryAutoAttackAnimationData is set to a valid DataAsset. */
	FAutoAttackManagerAnimationDataSignature OnPrimaryAutoAttackAnimationDataUpdatedDelegate;
	/** Called when the SecondaryAutoAttackAnimationData is set to a valid DataAsset. */
	FAutoAttackManagerAnimationDataSignature OnSecondaryAutoAttackAnimationDataUpdatedDelegate;
	
	UFUNCTION(BlueprintPure, Category = "AutoAttackManager|Animation")
	UAutoAttackAnimationData* GetPrimaryAutoAttackAnimationData() const;
	UFUNCTION(BlueprintPure, Category = "AutoAttackManager|Animation")
	UAutoAttackAnimationData* GetSecondaryAutoAttackAnimationData() const;
	
		
	UFUNCTION(BlueprintCallable, Category = "AutoAttackManager|Animation")
	void SetPrimaryAutoAttackAnimationData(UAutoAttackAnimationData* AnimationData);
	UFUNCTION(BlueprintCallable, Category = "AutoAttackManager|Animation")
	void SetSecondaryAutoAttackAnimationData(UAutoAttackAnimationData* AnimationData);

	UFUNCTION(BlueprintPure, Category = "AutoAttackManager")
	bool HasAuthority() const;
	
protected:
	
	UFUNCTION()
	void OnRep_AutoAttacking();

private:
	/** Attacks done from the main hand. */
	UPROPERTY(EditAnywhere, Category = "AutoAttackManager|Animation")
	TObjectPtr<UAutoAttackAnimationData> PrimaryAutoAttackAnimationData;
	/** Attacks done from the offhand when dual wielding. */
	UPROPERTY(EditAnywhere, Category = "AutoAttackManager|Animation")
	TObjectPtr<UAutoAttackAnimationData> SecondaryAutoAttackAnimationData;

	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(ReplicatedUsing=OnRep_AutoAttacking)
	bool bAutoAttacking = false;
	
	FTimerHandle AutoAttackTimer;
	
	/** The time in seconds between auto attacks. */
	float AutoAttackDelay = 1.f;
	
	bool bAutoAttackTimerPaused = false;
	
	/** Cached value of whether the owner is a simulated actor. */
	UPROPERTY()
	bool bCachedIsNetSimulated = false;
	void CacheIsNetSimulated();
	
	UFUNCTION()
	void ActivateAutoAttackAbility();
	
	void OnAutoAttackDelayAttributeChanged(const FOnAttributeChangeData& Data);
	void OnPauseAutoAttackTagChanged(const FGameplayTag Tag, int32 NewCount);
	
	UFUNCTION(Server, Reliable)
	void Server_StartAutoAttack();
	
	UFUNCTION(Server, Reliable)
	void Server_StopAutoAttack();
};
