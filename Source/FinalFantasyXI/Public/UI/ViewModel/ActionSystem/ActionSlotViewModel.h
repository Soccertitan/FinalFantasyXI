// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MVVMViewModelBase.h"
#include "ActionSlotViewModel.generated.h"


class UActionViewModel;
/**
 * Has a ActionViewModel nested within.
 */
UCLASS()
class FINALFANTASYXI_API UActionSlotViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UActionViewModel* GetActionViewModel() const {return ActionViewModel;}
	
protected:
	void SetActionViewModel(UActionViewModel* Value);
	
private:
	UPROPERTY()
	FGameplayTag InputTag;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|ActionBar", meta = (AllowPrivateAccess = true))
	TObjectPtr<UActionViewModel> ActionViewModel;
	
	friend class UActionManagerViewModel;
};
