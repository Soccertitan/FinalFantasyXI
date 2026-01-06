// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySet.h"
#include "CrimAbilitySystemInterface.h"
#include "HeroClassDefinition.h"
#include "HeroTypes.h"
#include "Components/ActorComponent.h"
#include "HeroManagerComponent.generated.h"

struct FOnAttributeChangeData;
class UHeroRaceDefinition;
class UHeroClassDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHeroClassProgressSignature, UHeroManagerComponent*, HeroManagerComponent,
                                             const FHeroClassProgressItem&, HeroClassProgressItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHeroClassLevelUpSignature, UHeroManagerComponent*, HeroManagerComponent,
											 const FHeroClassProgressItem&, HeroClassProgressItem, int32, OldLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHeroManagerGenericSignature, UHeroManagerComponent*, HeroManagerComponent);

/**
 * Applies abilities and attributes for the ASC based on the Race and Class combination for a character.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINALFANTASYXI_API UHeroManagerComponent : public UActorComponent, public ICrimAbilitySystemInterface
{
	GENERATED_BODY()

	friend struct FHeroClassProgressContainer;

public:
	UHeroManagerComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent) override;

	/** [Client + Server] whenever the HeroClass progress item is updated. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnHeroClassProgressUpdated")
	FHeroClassProgressSignature OnHeroClassProgressUpdatedDelegate;
	/** Called on Client + Server when the HeroClass changes. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnHeroClassChanged")
	FHeroManagerGenericSignature OnHeroClassChangedDelegate;
	/** Called on Client + Server when the HeroClass changes. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnHeroRaceChanged")
	FHeroManagerGenericSignature OnHeroRaceChangedDelegate;
	/** Called in an Unreliable Multicast event when the HeroClass increases it's level. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnHeroClassLevelUp")
	FHeroClassLevelUpSignature OnHeroClassLevelUpDelegate;

	/** Called on Server whenever the Hero's base stats have been overriden. */
	FHeroManagerGenericSignature OnHeroBaseStatsChangedDelegate;

	/** Called on the owning client if setting the HeroClass fails via the TrySetHeroClass function call. */
	FHeroManagerGenericSignature OnTrySetHeroClassFailedDelegate;

	/** @return A copy of all the HeroClassProgressItems. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "HeroManager")
	TArray<FHeroClassProgressItem> GetHeroClassProgressItems() const;

	/** Returns a copy of the HeroClassProgressItem for the specified HeroClass. */
	UFUNCTION(BlueprintPure, Category = "HeroManager")
	FHeroClassProgressItem FindHeroClassProgressItem(UPARAM(meta = (Categories = "HeroClass")) const FGameplayTag HeroClassTag) const;

	/**
	 * Adds a new HeroClassProgressItem or updates the existing one. Then, if the HeroClass Tag matches the progress tag,
	 * sets the level of the hero.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "HeroManager")
	void AddOrSetHeroClassProgress(const FHeroClassProgressItem& InHeroClassProgressItem);
	
	/**
	 * Adds the specified amount of experience for the current HeroClass. If it goes beyond the required exp for level
	 * it will add levels to the item.
	 * @param Experience The amount of experience to add.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "HeroManager")
	void AddExperience(const int32 Experience);

	/**
	 * Adds the specified amount of experience for the HeroClass. If it goes beyond the required exp for level it will
	 * add levels to the item.
	 * @param InHeroClass The HeroClass to add experience to.
	 * @param Experience The amount of experience to add.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "HeroManager")
	void AddExperienceForHeroClass(const UHeroClassDefinition* InHeroClass, const int32 Experience);

	/**
	 * Resets the current progress and then restores the progress to the HeroClassProgressContainer.
	 * @param InHeroClassProgressItems The progress to restore.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "HeroManager")
	void RestoreHeroClassProgressItems(const TArray<FHeroClassProgressItem>& InHeroClassProgressItems);

	/**
	 * Tries to change to the InHeroClass. If the HeroClass tag does not exist in the HeroClassProgressContainer
	 * the class will not be switched to.
	 * @param InHeroClass The HeroClass to switch to.
	 */
	UFUNCTION(BlueprintCallable, Category = "HeroManager")
	void TrySetHeroClass(UHeroClassDefinition* InHeroClass);

	/**
	 * Forcibly sets the HeroClass to the passed in value. If a HeroClassProgressItem does not exist, a new one will be
	 * created at Level 1.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "HeroManager")
	void SetHeroClass(UHeroClassDefinition* InHeroClass);

	/** Returns the currently set HeroClass.*/
	UFUNCTION(BlueprintPure, Category = "HeroManager")
	UHeroClassDefinition* GetHeroClass() const { return HeroClass; }
	
	/** Changes the HeroRace to the passed in value. */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "HeroManager")
	void SetHeroRace(UHeroRaceDefinition* InHeroRace);

	/** Returns the currently set HeroRace.*/
	UFUNCTION(BlueprintPure, Category = "HeroManager")
	UHeroRaceDefinition* GetHeroRace() const { return HeroRace; }
	
	/** @return The max level that can be obtained. */
	UFUNCTION(BlueprintPure, Category = "HeroManager")
	int32 GetMaxLevel() const {return MaxLevel;}

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "HeroManager")
	void SetMaxLevel(const int32 InMaxLevel);
	
	UFUNCTION(BlueprintPure, Category = "HeroManager")
	bool HasAuthority() const;

protected:
	virtual void BeginPlay() override;
	virtual void PreNetReceive() override;
	virtual void OnRegister() override;

	// Called on server when the Level Attribute changes.
	virtual void OnLevelAttributeChanged(const FOnAttributeChangeData& Data);

	UFUNCTION()
	virtual void OnRep_HeroClass();
	UFUNCTION()
	virtual void OnRep_HeroRace();

private:
	/** The current HeroRace the character is. */
	UPROPERTY(EditAnywhere, ReplicatedUsing=Onrep_HeroRace, Category = "HeroManager")
	TObjectPtr<UHeroRaceDefinition> HeroRace;
	
	/** The current HeroClass the character is. */
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_HeroClass, Category = "HeroManager")
	TObjectPtr<UHeroClassDefinition> HeroClass;

	/** Contains the replicated container of character's HeroProgress. */
	UPROPERTY(Replicated)
	FHeroClassProgressContainer HeroClassProgressContainer;

	/** The max level any item in the HeroClassProgressContainer can achieve. */
	UPROPERTY(EditDefaultsOnly, Replicated, Category = "HeroManager")
	int32 MaxLevel = 20;

	/** Cached ASC from the owner. */
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;

	/** Cached value of whether our owner is a simulated Actor. */
	UPROPERTY()
	bool bCachedIsNetSimulated = false;
	void CacheIsNetSimulated();

	/** Sets the Level Attribute in the ASC to the passed in Level. */
	UFUNCTION()
	void SetHeroClassLevel(const int32 Level);

	/**
	 * Called when the class, race, or the level attribute changes.
	 * Overrides the base attributes on the ASC with the base attribute values from the HeroClass and HeroRace.
	 */
	void ApplyBaseAttributes(const float Level);

	/** Stored AbilitySet handles from the HeroRace Traits. */
	UPROPERTY()
	FAbilitySet_GrantedHandles AbilitySet_GrantedHandles_RaceTraits;
	/** Grants the AbilitySet stored in the HeroRaceDefinition. */
	void ApplyHeroRaceTraits();

	/** Stored AbilitySet handles from the HeroClass Traits. */
	UPROPERTY()
	FAbilitySet_GrantedHandles AbilitySet_GrantedHandles_ClassTraits;
	/** Grants the AbilitySet stored in the HeroClassDefinition. */
	void ApplyHeroClassTraits();

	UFUNCTION(Server, Reliable)
	void Server_TrySetHeroClass(const UHeroClassDefinition* InHeroClass);

	UFUNCTION(Client, Reliable)
	void Client_TrySetHeroClassFailed();

	/** Calls Multicast event Multi_OnLevelUp and updates the current level of the hero if applicable. */
	void Internal_OnLevelUp(const FHeroClassProgressItem& HeroClassProgressItem, const int32 OldLevel);

	/** Broadcasts the OnLevelUp delegate. */
	UFUNCTION(NetMulticast, Unreliable)
	void Multi_OnLevelUp(const FHeroClassProgressItem& HeroClassProgressItem, const int32 OldLevel);
};
