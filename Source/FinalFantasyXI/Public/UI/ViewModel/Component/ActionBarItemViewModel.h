// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MVVMViewModelBase.h"
#include "ActionBarItemViewModel.generated.h"

class UGameplayAbilityDefinitionViewModel;
class UGameplayAbilityDefinition;

/**
 * Informs the ability at the given InputTag and if it's pressed or not.
 */
UCLASS()
class FINALFANTASYXI_API UActionBarItemViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	bool IsInputPressed() const { return bInputPressed; }
	FGameplayTag GetInputTag() const { return InputTag; }

	void SetInputTag(FGameplayTag InValue);
	void SetIsInputPressed(bool InValue);
	void SetAbilityDefinition(UGameplayAbilityDefinition* AbilityDefinition);

protected:

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, meta = (AllowPrivateAccess = "true"))
	FGameplayTag InputTag;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter=IsInputPressed, meta = (AllowPrivateAccess = "true"))
	bool bInputPressed = false;

	UPROPERTY(BlueprintReadOnly, FieldNotify, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGameplayAbilityDefinitionViewModel> AbilityDefinitionViewModel;
};
