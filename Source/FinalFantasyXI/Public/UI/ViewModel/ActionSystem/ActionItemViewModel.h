// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MVVMViewModelBase.h"
#include "Engine/StreamableManager.h"
#include "ActionItemViewModel.generated.h"

class UActionManagerViewModel;
class UCrysAction;

/**
 * Contains information of an action.
 */
UCLASS()
class FINALFANTASYXI_API UActionItemViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	FGameplayTag GetInputTag() const { return InputTag; }
	FText GetActionName() const { return ActionName; }
	UTexture2D* GetIcon() const { return Icon; }
	
	void SetInputTag(const FGameplayTag& InInputTag);
	void SetAction(UCrysAction* InAction);

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, Category = "ActionBarItemViewModel", meta = (AllowPrivateAccess = true))
	FGameplayTag InputTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, Category = "ActionBarItemViewModel", meta = (AllowPrivateAccess = true))
	FText ActionName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, Category = "ActionBarItemViewModel", meta = (AllowPrivateAccess = true))
	TObjectPtr<UTexture2D> Icon;
	
	UPROPERTY()
	TObjectPtr<UCrysAction> Action;
	
	void OnActionObjectsLoaded();
	TSharedPtr<FStreamableHandle> ActionObjectsStreamableHandle;
	
	friend UActionManagerViewModel;
};
