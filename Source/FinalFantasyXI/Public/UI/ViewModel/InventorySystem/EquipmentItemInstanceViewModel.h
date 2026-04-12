// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrysItemInstanceViewModel.h"
#include "EquipmentItemInstanceViewModel.generated.h"

class UCrysGameplayTagViewModel;
/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UEquipmentItemInstanceViewModel : public UCrysItemInstanceViewModel
{
	GENERATED_BODY()
	
public:
	int32 GetLevelRequirement() const {return LevelRequirement;}
	UFUNCTION(BlueprintPure, FieldNotify)
	TArray<UCrysGameplayTagViewModel*> GetAllowedJobViewModels() const {return AllowedJobViewModels;}
	UFUNCTION(BlueprintPure, FieldNotify)
	UCrysGameplayTagViewModel* GetEquipSlotViewModels() const {return EquipSlotViewModel;}
	
protected:
	void SetLevelRequirement(int32 Value);
	void SetAllowedJobViewModels(TArray<UCrysGameplayTagViewModel*> Value);
	void SetEquipSlotViewModels(UCrysGameplayTagViewModel* Value);
	
	virtual void OnItemInstanceSet() override;
	virtual void OnItemDefinitionLoaded(const UItemDefinition* ItemDefinition) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	int32 LevelRequirement = 0;

	UPROPERTY()
	TObjectPtr<UCrysGameplayTagViewModel> EquipSlotViewModel;
	
	UPROPERTY()
	TArray<TObjectPtr<UCrysGameplayTagViewModel>> AllowedJobViewModels;
};
