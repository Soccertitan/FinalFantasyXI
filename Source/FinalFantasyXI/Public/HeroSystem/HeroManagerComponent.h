// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySet.h"
#include "CrimAbilitySystemInterface.h"
#include "HeroJobDefinition.h"
#include "HeroTypes.h"
#include "Components/ActorComponent.h"
#include "HeroManagerComponent.generated.h"

struct FGameplayAttribute;
struct FOnAttributeChangeData;
class UHeroRaceDefinition;
class UHeroJobDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHeroManagerGenericSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHeroManagerBoolSignature, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHeroManagerCharacterLevelUpdatedGenericSignature, int32, OldLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHeroJobProgressSignature, const FHeroJobProgressItem&, HeroJobProgressItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHeroJobLevelUpSignature, const FHeroJobProgressItem&, HeroJobProgressItem, int32, OldLevel);

/**
 * Applies abilities and attributes for the ASC based on the Race and Job combination.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINALFANTASYXI_API UHeroManagerComponent : public UActorComponent, public ICrimAbilitySystemInterface
{
	GENERATED_BODY()

	friend struct FHeroJobProgressContainer;

public:
	UHeroManagerComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent) override;

	/** [Client + Server] Whenever the HeroProgress is updated. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnHeroProgressUpdated")
	FHeroManagerGenericSignature OnHeroProgressUpdatedDelegate;
	/** [Server + UnreliableMulticast] When the HeroCharacter levels up. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnHeroLevelUp")
	FHeroManagerCharacterLevelUpdatedGenericSignature OnHeroLevelUpDelegate;
	/** [Server + UnreliableMulticast] When the HeroCharacter levels down. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnHeroLevelDown")
	FHeroManagerCharacterLevelUpdatedGenericSignature OnHeroLevelDownDelegate;

	/** [Server + UnreliableMulticast] Called when a HeroJob levels up. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnHeroJobLevelUp")
	FHeroJobLevelUpSignature OnHeroJobLevelUpDelegate;
	/** [Client + Server] whenever a HeroJobProgressItem is updated. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnHeroJobProgressUpdated")
	FHeroJobProgressSignature OnHeroJobProgressUpdatedDelegate;
	/** [Client + Server] When the MainJob changes. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnHeroMainJobChanged")
	FHeroManagerGenericSignature OnHeroMainJobChangedDelegate;
	/** [Client + Server] When the SubJob changes. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnHeroSubJobChanged")
	FHeroManagerGenericSignature OnHeroSubJobChangedDelegate;
	
	/** [Client + Server] When the HeroRace changes. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnHeroRaceChanged")
	FHeroManagerGenericSignature OnHeroRaceChangedDelegate;

	/** [Owning Client] When setting the HeroJobs succeeds or fails. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnTrySetHeroJob")
	FHeroManagerBoolSignature OnTrySetHeroJobDelegate;
	
	/** @return A copy of all the HeroClassProgressItems. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "HeroManager")
	TArray<FHeroJobProgressItem> GetHeroJobProgressItems() const;
	
	/** Returns a copy of the HeroClassProgressItem for the specified HeroClass. */
	UFUNCTION(BlueprintPure, Category = "HeroManager")
	FHeroJobProgressItem FindHeroJobProgressItem(UPARAM(meta = (Categories = "HeroJob")) const FGameplayTag HeroJobTag) const;
	
	/**
	 * Adds a new HeroJobProgressItem or updates an existing one. Then, if the JobTag matches the currently equipped jobs
	 * update the attributes.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "HeroManager")
	void AddOrSetHeroJobProgressItem(const FHeroJobProgressItem& InHeroJobProgressItem);
	
	/**
	 * Adds the specified amount of experience for the HeroClass. If it goes beyond the required exp for level it will
	 * add levels to the item.
	 * @param HeroJob The HeroClass to add experience to.
	 * @param Experience The amount of experience to add.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "HeroManager")
	void AddExperienceForHeroJob(const UHeroJobDefinition* HeroJob, const int32 Experience);
	
	/**
	 * Resets the current progress and then restores the progress to the HeroClassProgressContainer.
	 * @param InHeroJobProgressItems The progress to restore.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "HeroManager")
	void RestoreHeroJobProgressItems(const TArray<FHeroJobProgressItem>& InHeroJobProgressItems);
	
	/**
	 * Tries to change into the HeroJobs. If the HeroJob tag does not exist in the HeroJobProgressContainer
	 * the switch is invalid and not occur.
	 */
	UFUNCTION(BlueprintCallable, Category = "HeroManager")
	void TrySetHeroJobs(UHeroJobDefinition* InMainJob, UHeroJobDefinition* InSubJob = nullptr);

	/** Returns the currently set HeroMainJob.*/
	UFUNCTION(BlueprintPure, Category = "HeroManager")
	UHeroJobDefinition* GetHeroMainJob() const { return MainJob; }
	/** Returns the currently set HeroSubJob.*/
	UFUNCTION(BlueprintPure, Category = "HeroManager")
	UHeroJobDefinition* GetHeroSubJob() const { return SubJob; }
	
	/** Changes the HeroRace to the passed in value. */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "HeroManager")
	void SetHeroRace(UHeroRaceDefinition* InHeroRace);
	
	/** Returns the currently set HeroRace.*/
	UFUNCTION(BlueprintPure, Category = "HeroManager")
	UHeroRaceDefinition* GetHeroRace() const { return HeroRace; }
	
	/** @return The current HeroProgress. */
	UFUNCTION(BlueprintPure, Category = "HeroManager")
	const FHeroProgress& GetHeroProgress() const;
	
	/** Updates the HeroProgress with the passed in item. */
	UFUNCTION(BlueprintCallable, Category = "HeroManager")
	void SetHeroProgress(const FHeroProgress& InHeroProgress);
	
	/**
	 * Adds the specified amount of experience for the character. If it goes beyond the required exp for level
	 * it will add levels and call the LevelUp or LevelDown event.
	 * @param Experience The amount of experience to add.
	 * @return True, if the level changed.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "HeroManager")
	bool AddExperience(const int32 Experience);
	
	UFUNCTION(BlueprintPure, Category = "HeroManager")
	bool HasAuthority() const;

protected:
	virtual void BeginPlay() override;
	virtual void PreNetReceive() override;
	virtual void OnRegister() override;

	UFUNCTION()
	void OnRep_HeroRace();
	UFUNCTION()
	void OnRep_HeroProgress();
	UFUNCTION()
	void OnRep_MainJob();
	UFUNCTION()
	void OnRep_SubJob();

private:
	/** The current HeroRace the character is. */
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_HeroRace, Category = "HeroManager")
	TObjectPtr<UHeroRaceDefinition> HeroRace;
	
	/** The overall progress of the hero. */
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_HeroProgress, Category = "HeroManager")
	FHeroProgress HeroProgress;
	
	/** The currently chosen MainJob. */
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_MainJob, Category = "HeroManager")
	TObjectPtr<UHeroJobDefinition> MainJob;
	
	/** The current chosen SubJob. */
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_SubJob, Category = "HeroManager")
	TObjectPtr<UHeroJobDefinition> SubJob;

	/** Contains the replicated container of character's Job progress. */
	UPROPERTY(Replicated)
	FHeroJobProgressContainer HeroJobProgressContainer;
	
	/** Initializes the HeroJobProgress with these values. */
	UPROPERTY(EditAnywhere, Category = "HeroManager")
	TArray<FHeroJobProgressItem> StartingJobProgress;

	/** Cached ASC from the owner. */
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	
	/** If true, the BaseAttribute will not be updated. Set to true when changing multiple jobs and levels at the same time. */
	UPROPERTY()
	bool bSkipApplyingBaseStats = false;

	/** Cached value of whether our owner is a simulated Actor. */
	UPROPERTY()
	bool bCachedIsNetSimulated = false;
	void CacheIsNetSimulated();

	/** Overrides the base value of an Attribute. */
	UFUNCTION()
	void OverrideAttribute(const float Value, const FGameplayAttribute& Attribute);
	
	/**
	 * Called when anything attributes related to the job or race changes. Or if a different race/job is set.
	 * Overrides the base attributes on the ASC with the base attribute values from the HeroJobs and HeroRace.
	 */
	void ApplyBaseAttributes();
	
	/** Stored AbilitySet handles from the HeroRace Traits. */
	UPROPERTY()
	FAbilitySet_GrantedHandles AbilitySet_GrantedHandles_RaceTraits;
	/** Grants the AbilitySet stored in the HeroRaceDefinition. */
	void ApplyHeroRaceTraits();
	
	/** Stored AbilitySet handle from the MainJob Traits. */
	UPROPERTY()
	FAbilitySet_GrantedHandles AbilitySet_GrantedHandles_MainJobTraits;
	/** Grants the AbilitySet stored in the HeroJobDefinition. */
	void ApplyHeroMainJobTraits();
	
	/** Stored AbilitySet handle from the SubJob Traits. */
	UPROPERTY()
	FAbilitySet_GrantedHandles AbilitySet_GrantedHandles_SubJobTraits;
	/** Grants the AbilitySet stored in the HeroJobDefinition. */
	void ApplyHeroSubJobTraits();
	
	void BindToAttributeDelegates();
	void RemoveBindToAttributeDelegates();
	
	// Called on server when an Attribute changes.
	virtual void OnAttributeChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_OnHeroLevelUp(const int32 OldLevel);
	UFUNCTION(NetMulticast, Unreliable)
	void Multi_OnHeroLevelDown(const int32 OldLevel);

	/** Calls Multicast event Multi_OnLevelUp and updates the current level of the hero if applicable. */
	void Internal_OnJobLevelUp(const FHeroJobProgressItem& HeroJobProgressItem, const int32 OldLevel);

	/** Broadcasts the OnLevelUp delegate. */
	UFUNCTION(NetMulticast, Unreliable)
	void Multi_OnJobLevelUp(const FHeroJobProgressItem& HeroJobProgressItem, const int32 OldLevel);
	
	UFUNCTION(Server, Reliable)
	void Server_TrySetHeroJobs(const UHeroJobDefinition* InMainJob, const UHeroJobDefinition* InSubJob);
	
	UFUNCTION(Client, Reliable)
	void Client_OnTrySetHeroJobs(bool bSuccess);
};
