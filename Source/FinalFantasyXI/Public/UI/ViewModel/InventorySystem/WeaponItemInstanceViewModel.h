// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "EquipmentItemInstanceViewModel.h"
#include "ScalableFloat.h"
#include "WeaponItemInstanceViewModel.generated.h"

class UCrysGameplayTagViewModel;

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UWeaponItemInstanceViewModel : public UEquipmentItemInstanceViewModel
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, FieldNotify)
	int32 GetDamage() const {return Damage.GetValueAtLevel(GetUpgradeLevel());}
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetAutoAttackDelay() const {return AutoAttackDelay.GetValueAtLevel(GetUpgradeLevel());}
	UCrysGameplayTagViewModel* GetWeaponSkillViewModel() const {return WeaponSkillViewModel;}
	UCrysGameplayTagViewModel* GetDamageTypeViewModel() const {return DamageTypeViewModel;}
	
protected:
	void SetDamage(FScalableFloat Value);
	void SetAutoAttackDelay(FScalableFloat Value);
	void SetWeaponSkillViewModel(UCrysGameplayTagViewModel* Value);
	void SetDamageTypeViewModel(UCrysGameplayTagViewModel* Value);
	
	virtual void OnItemInstanceSet() override;
	virtual void OnItemDefinitionLoaded(const UItemDefinition* ItemDefinition) override;
	
private:
	UPROPERTY(EditAnywhere)
	FScalableFloat Damage;
	
	UPROPERTY(EditAnywhere)
	FScalableFloat AutoAttackDelay;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCrysGameplayTagViewModel> WeaponSkillViewModel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCrysGameplayTagViewModel> DamageTypeViewModel;
};
