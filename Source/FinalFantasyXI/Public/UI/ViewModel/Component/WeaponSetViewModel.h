// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MVVMViewModelBase.h"
#include "Player/Hero/WeaponSetTypes.h"
#include "WeaponSetViewModel.generated.h"

class UGameplayAbilityDefinitionViewModel;
class UItemInstanceViewModel;

/**
 * Contains information on a WeaponSet, includes the weapon and ability details.
 */
UCLASS()
class FINALFANTASYXI_API UWeaponSetViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	int32 GetWeaponSetIndex() const { return WeaponSetIndex; }
	bool CanEquipWeapon() const { return bCanEquipWeapon; }
	UItemInstanceViewModel* GetWeaponViewModel() const { return WeaponViewModel; }

	UFUNCTION(BlueprintCallable, Category = "WeaponSetViewModel")
	UGameplayAbilityDefinitionViewModel* FindOrCreateGameplayAbilityDefinitionViewModel(UPARAM(meta = (Categories="Input")) FGameplayTag InputTag);

	void SetWeaponSetIndex(const int32 InValue);
	void SetWeaponSet(const FWeaponSet& InWeaponSet);
	void SetWeaponViewModel(UItemInstanceViewModel* InViewModel);

	FWeaponSet GetWeaponSet() const { return WeaponSet; }
	
protected:
	void SetCanEquipWeapon(const bool InValue);

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "WeaponSetViewModel", meta=(AllowPrivateAccess=true))
	int32 WeaponSetIndex = INDEX_NONE;

	//** True, if the weapon in the WeaponSet equipable. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter=CanEquipWeapon, Category = "WeaponSetViewModel", meta=(AllowPrivateAccess=true))
	bool bCanEquipWeapon = false;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "WeaponSetViewModel", meta=(AllowPrivateAccess = true))
	TObjectPtr<UItemInstanceViewModel> WeaponViewModel;

	/** InputTag to AbilityDefinitionMap. */
	UPROPERTY(BlueprintReadOnly, Category = "WeaponSetViewModel", meta=(AllowPrivateAccess=true))
	TMap<FGameplayTag, TObjectPtr<UGameplayAbilityDefinitionViewModel>> GameplayAbilityDefinitionInputMapViewModels;

	/** Cached copy of the WeaponSet. */
	UPROPERTY()
	FWeaponSet WeaponSet;
};
