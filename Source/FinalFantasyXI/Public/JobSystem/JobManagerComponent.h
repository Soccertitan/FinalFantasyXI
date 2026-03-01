// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySet.h"
#include "CrimAbilitySystemInterface.h"
#include "JobDefinition.h"
#include "JobTypes.h"
#include "Components/ActorComponent.h"
#include "JobManagerComponent.generated.h"

struct FGameplayAttribute;
struct FOnAttributeChangeData;
class URaceDefinition;
class UJobDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJobManagerGenericSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJobManagerBoolSignature, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJobManagerCharacterLevelUpdatedGenericSignature, int32, OldLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJobManagerJobProgressSignature, const FJobProgressItem&, JobProgressItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FJobManagerJobLevelUpSignature, const FJobProgressItem&, JobProgressItem, int32, OldLevel);

/**
 * Applies abilities and attributes for the ASC based on the Race and Job combination.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINALFANTASYXI_API UJobManagerComponent : public UActorComponent, public ICrimAbilitySystemInterface
{
	GENERATED_BODY()

	friend struct FJobProgressContainer;

public:
	UJobManagerComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent) override;

	/** [Client + Server] Whenever the JobManagerData is updated. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnJobManagerDataUpdated")
	FJobManagerGenericSignature OnJobManagerDataUpdatedDelegate;
	/** [Server + UnreliableMulticast] When the Character levels up. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnLevelUp")
	FJobManagerCharacterLevelUpdatedGenericSignature OnLevelUpDelegate;
	/** [Server + UnreliableMulticast] When the Character levels down. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnLevelDown")
	FJobManagerCharacterLevelUpdatedGenericSignature OnLevelDownDelegate;

	/** [Server + UnreliableMulticast] Called when a Job levels up. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnJobLevelUp")
	FJobManagerJobLevelUpSignature OnJobLevelUpDelegate;
	/** [Client + Server] whenever a JobProgressItem is updated. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnJobProgressUpdated")
	FJobManagerJobProgressSignature OnJobProgressUpdatedDelegate;
	/** [Client + Server] When the MainJob changes. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnMainJobChanged")
	FJobManagerGenericSignature OnMainJobChangedDelegate;
	/** [Client + Server] When the SubJob changes. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnSubJobChanged")
	FJobManagerGenericSignature OnSubJobChangedDelegate;
	
	/** [Client + Server] When the Race changes. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnRaceChanged")
	FJobManagerGenericSignature OnRaceChangedDelegate;

	/** [Owning Client] When setting the Jobs succeeds or fails. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnTrySetJob")
	FJobManagerBoolSignature OnTrySetJobDelegate;
	
	/** @return A copy of all the JobProgressItems. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "JobManager")
	TArray<FJobProgressItem> GetJobProgressItems() const;
	
	/** Returns a copy of the JobProgressItem. */
	UFUNCTION(BlueprintPure, Category = "JobManager")
	FJobProgressItem FindJobProgressItem(UPARAM(meta = (Categories = "Job")) const FGameplayTag JobTag) const;
	
	/**
	 * Adds a new JobProgressItem or updates an existing one. Then, if the JobTag matches the currently equipped jobs
	 * update the attributes.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "JobManager")
	void AddOrSetJobProgressItem(const FJobProgressItem& InJobProgressItem);
	
	/**
	 * Adds the specified amount of experience for the Job. If it goes beyond the required exp for level it will
	 * add levels to the item.
	 * @param Job The job to add experience to.
	 * @param Experience The amount of experience to add.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "JobManager")
	void AddExperienceForJob(const UJobDefinition* Job, const int32 Experience);
	
	/**
	 * Resets the current progress and then restores the progress to the JobProgressContainer.
	 * @param InJobProgressItems The progress to restore.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "JobManager")
	void RestoreJobProgressItems(const TArray<FJobProgressItem>& InJobProgressItems);
	
	/**
	 * Tries to change into the Jobs.
	 */
	UFUNCTION(BlueprintCallable, Category = "JobManager")
	void TrySetJobs(UJobDefinition* InMainJob, UJobDefinition* InSubJob = nullptr);

	UFUNCTION(BlueprintPure, Category = "JobManager")
	UJobDefinition* GetMainJob() const { return MainJob; }
	UFUNCTION(BlueprintPure, Category = "JobManager")
	UJobDefinition* GetSubJob() const { return SubJob; }
	
	/** Changes the Race to the passed in value. */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "JobManager")
	void SetRaceDefinition(URaceDefinition* InRaceDefinition);
	
	/** Returns the currently set Race.*/
	UFUNCTION(BlueprintPure, Category = "JobManager")
	URaceDefinition* GetRaceDefinition() const { return RaceDefinition; }

	UFUNCTION(BlueprintPure, Category = "JobManager")
	const FJobManagerData& GetJobManagerData() const;
	
	/** Updates the JobManagerData with the passed in item. */
	UFUNCTION(BlueprintCallable, Category = "JobManager")
	void SetJobManagerData(const FJobManagerData& InData);
	
	/**
	 * Adds the specified amount of experience for the character to the JobManagerData. If it goes beyond the required exp for level
	 * it will add levels and call the LevelUp or LevelDown event.
	 * @param Experience The amount of experience to add.
	 * @return True, if the level changed.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "JobManager")
	bool AddExperience(const int32 Experience);
	
	UFUNCTION(BlueprintPure, Category = "JobManager")
	bool HasAuthority() const;

protected:
	virtual void BeginPlay() override;
	virtual void PreNetReceive() override;
	virtual void OnRegister() override;

	UFUNCTION()
	void OnRep_RaceDefinition();
	UFUNCTION()
	void OnRep_JobManagerData();
	UFUNCTION()
	void OnRep_MainJob();
	UFUNCTION()
	void OnRep_SubJob();

private:
	/** The current Race the character is. */
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_RaceDefinition, Category = "JobManager")
	TObjectPtr<URaceDefinition> RaceDefinition;
	
	/** The overall progress of the JobManager. */
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_JobManagerData, Category = "JobManager")
	FJobManagerData JobManagerData;
	
	/** The currently chosen MainJob. */
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_MainJob, Category = "JobManager")
	TObjectPtr<UJobDefinition> MainJob;
	
	/** The current chosen SubJob. */
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_SubJob, Category = "JobManager")
	TObjectPtr<UJobDefinition> SubJob;

	/** Contains the replicated container of character's Job progress. */
	UPROPERTY(Replicated)
	FJobProgressContainer JobProgressContainer;
	
	/** Initializes JobProgress with these values. */
	UPROPERTY(EditAnywhere, Category = "JobManager")
	TArray<FJobProgressItem> StartingJobProgress;

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
	 * Overrides the base attributes on the ASC with the base attribute values from the equipped Jobs and Race.
	 */
	void ApplyBaseAttributes();
	
	/** Stored AbilitySet handles from the RaceDefinition Traits. */
	UPROPERTY()
	FAbilitySet_GrantedHandles AbilitySet_GrantedHandles_RaceTraits;
	/** Grants the AbilitySet stored in the RaceDefinition. */
	void ApplyRaceTraits();
	
	/** Stored AbilitySet handle from the MainJob Traits. */
	UPROPERTY()
	FAbilitySet_GrantedHandles AbilitySet_GrantedHandles_MainJobTraits;
	/** Grants the AbilitySet stored in the JobDefinition. */
	void ApplyMainJobTraits();
	
	/** Stored AbilitySet handle from the SubJob Traits. */
	UPROPERTY()
	FAbilitySet_GrantedHandles AbilitySet_GrantedHandles_SubJobTraits;
	/** Grants the AbilitySet stored in the JobDefinition. */
	void ApplySubJobTraits();
	
	void BindToAttributeDelegates();
	void RemoveBindToAttributeDelegates();
	
	// Called on server when an Attribute changes.
	virtual void OnAttributeChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_OnLevelUp(const int32 OldLevel);
	UFUNCTION(NetMulticast, Unreliable)
	void Multi_OnLevelDown(const int32 OldLevel);

	/** Calls Multicast event Multi_OnLevelUp and updates the current level of the Job if applicable. */
	void Internal_OnJobLevelUp(const FJobProgressItem& JobProgressItem, const int32 OldLevel);

	/** Broadcasts the OnLevelUp delegate. */
	UFUNCTION(NetMulticast, Unreliable)
	void Multi_OnJobLevelUp(const FJobProgressItem& JobProgressItem, const int32 OldLevel);
	
	UFUNCTION(Server, Reliable)
	void Server_TrySetJobs(const UJobDefinition* InMainJob, const UJobDefinition* InSubJob);
	
	UFUNCTION(Client, Reliable)
	void Client_OnTrySetJobs(bool bSuccess);
};
