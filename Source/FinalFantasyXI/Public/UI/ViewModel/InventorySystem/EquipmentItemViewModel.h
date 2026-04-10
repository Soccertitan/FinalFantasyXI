// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrysItemViewModel.h"
#include "EquipmentItemViewModel.generated.h"

class UCrysGameplayTagViewModel;

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UEquipmentItemViewModel : public UCrysItemViewModel
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
	
	virtual void OnItemSet(const TInstancedStruct<FItem>& Item) override;
	virtual void OnItemDefinitionSet(const UItemDefinition* ItemDefinition) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	int32 LevelRequirement = 0;

	UPROPERTY()
	TObjectPtr<UCrysGameplayTagViewModel> EquipSlotViewModel;
	
	UPROPERTY()
	TArray<TObjectPtr<UCrysGameplayTagViewModel>> AllowedJobViewModels;
};
