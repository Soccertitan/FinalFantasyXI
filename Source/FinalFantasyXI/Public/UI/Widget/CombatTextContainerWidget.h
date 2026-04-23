// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrysUserWidget.h"
#include "GameplayTagContainer.h"
#include "CombatTextContainerWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FINALFANTASYXI_API UCombatTextContainerWidget : public UCrysUserWidget
{
	GENERATED_BODY()
	
public:
	
	/** Pushes combat text to the widget. */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "CombatText")
	void PushCombatText(const FGameplayTagContainer& StyleTags, const FText& Text);
};
