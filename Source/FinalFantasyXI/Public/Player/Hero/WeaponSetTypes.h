// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Input/AbilityInputTypes.h"

#include "WeaponSetTypes.generated.h"


/** A struct with an ItemGuid and an array of AbilityInputItems managed by the WeaponSetManagerComponent. */
USTRUCT(BlueprintType)
struct FWeaponSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGuid ItemGuid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FAbilityInputItem> InputItems;

	/** Is true, if the ItemGuid can be equipped in the weapon EquipSlot. */
	bool CanEquipWeapon() const { return bCanEquipWeapon; }
	
private:
	/** Is true, the ItemGuid can be equipped in the weapon EquipSlot. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCanEquipWeapon = false;

	friend class UWeaponSetManagerComponent;
};
