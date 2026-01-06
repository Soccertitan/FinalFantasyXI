// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "WeaponSetTypes.h"
#include "Components/ActorComponent.h"
#include "EquipmentSystem/EquipmentManagerComponent.h"
#include "WeaponSetManagerComponent.generated.h"


class UAbilityInputManagerComponent;
class UEquipmentManagerComponent;

DECLARE_MULTICAST_DELEGATE_TwoParams(FWeaponSetManagerComponentActiveWeaponSetChangedSignature, int32 /** NewValue */, int32 /** OldValue */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FWeaponSetmanagerComponentWeaponSetChangedSignature, const FWeaponSet& /** The updated WeaponSet */, int32 /** Index in the array */);

/**
 * This class bridges the gap between equipping weapons and assigning abilities to InputTags for the equipped weapon.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINALFANTASYXI_API UWeaponSetManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponSetManagerComponent();

	/** Called when the active WeaponSetIndex has changed. NewValue first, and the OldValue is second. */
	FWeaponSetManagerComponentActiveWeaponSetChangedSignature OnActiveWeaponSetChangedDelegate;
	/** Called when a weapon set changes. */
	FWeaponSetmanagerComponentWeaponSetChangedSignature OnWeaponSetChangedDelegate;

	/** Readies the WeaponSetManager with the requirements to start functioning properly. */
	UFUNCTION(BlueprintCallable, Category = "WeaponSetManager")
	void InitializeWeaponSetManager(UAbilityInputManagerComponent* InAbilityInputManagerComponent, UEquipmentManagerComponent* InEquipmentManagerComponent);

	UFUNCTION(BlueprintPure, Category = "WeaponSetManager")
	bool IsWeaponSetManagerInitialized() const;

	/** Retrieves the WeaponSetItem from the Index and applies it to the AbilityInputManagerComponent. */
	UFUNCTION(BlueprintCallable, Category = "WeaponSetManager")
	void SetActiveWeaponSet(int32 Index = 0);

	/** Retrieves the ActiveWeaponSetIndex, -1 is no set is active. */
	UFUNCTION(BlueprintCallable, Category = "WeaponSetManager")
	int32 GetActiveWeaponSetIndex() const {return ActiveWeaponSetIndex;}

	/** Returns a copy of the WeaponSet at the index. */
	UFUNCTION(BlueprintPure, Category = "WeaponSetManager")
	FWeaponSet GetWeaponSetAt(const int32 Index = 0) const;

	/**
	 * Adds a WeaponSet at Index and replaces an existing one if it already exists.
	 * If Index is less than 0, appends it to end of the array.
	 */
	UFUNCTION(BlueprintCallable, Category = "WeaponSetManager")
	void AddWeaponSet(UPARAM(ref) FWeaponSet& InWeaponSet, const int32 Index = 0);

	/** Nulls out the WeaponSet at Index. Does not remove it from the WeaponSetContainer. */
	UFUNCTION(BlueprintCallable, Category = "WeaponSetManager")
	void ClearWeaponSet(const int32 Index = 0);

	/** Appends the InWeaponSets to the array. */
	UFUNCTION(BlueprintCallable, Category = "WeaponSetManager")
	void AddWeaponSets(UPARAM(ref) TArray<FWeaponSet>& InWeaponSets);

	/** Removes all WeaponSets from the WeaponSetContainer and nulls out the ActiveWeaponSet. */
	UFUNCTION(BlueprintCallable, Category = "WeaponSetManager")
	void RemoveAllWeaponSets();

	/** Checks and updates all WeaponSets if the Item can be equipped. */
	UFUNCTION(BlueprintCallable, Category = "WeaponSetManager")
	void UpdateEquipStatus();

	/** Returns a const reference to the WeaponSets array. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "WeaponSetManager")
	const TArray<FWeaponSet>& GetWeaponSets() const;

private:
	/** The index of the currently applied WeaponSetItem. */
	UPROPERTY()
	int32 ActiveWeaponSetIndex = -1;
	/** The mapping of weapons to inputs. */
	UPROPERTY()
	TArray<FWeaponSet> WeaponSets;

	/** Cached AbilityInputManagerComponent from the Player Controller. */
	UPROPERTY()
	TObjectPtr<UAbilityInputManagerComponent> AbilityInputManagerComponent;
	/** Cached EquipmentManagerComponent from the Pawn/PlayerState. */
	UPROPERTY()
	TObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;

	void Internal_UpdateWeaponSet(FWeaponSet& WeaponSet, int32 Index);
	void Internal_ResetWeaponSets();
};
