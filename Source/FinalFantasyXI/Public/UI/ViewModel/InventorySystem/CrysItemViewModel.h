// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/ItemViewModel.h"
#include "CrysItemViewModel.generated.h"

/**
 * The root ItemViewModel for all items in this project.
 */
UCLASS()
class FINALFANTASYXI_API UCrysItemViewModel : public UItemViewModel
{
	GENERATED_BODY()
	
public:
	int32 GetUpgradeLevel() const { return UpgradeLevel; }
	bool IsEquipped() const { return bEquipped;}
	
protected:
	void SetUpgradeLevel(int32 Value);
	void SetIsEquipped(bool bValue);
	
private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	int32 UpgradeLevel = 0;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter=IsEquipped, meta = (AllowPrivateAccess = "true"))
	bool bEquipped = false;
};
