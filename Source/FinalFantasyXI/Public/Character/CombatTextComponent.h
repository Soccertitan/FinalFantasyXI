// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CombatTextComponent.generated.h"


struct FGameplayTagContainer;
struct FGameplayTag;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINALFANTASYXI_API UCombatTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UCombatTextComponent();
	
	/** Pushes combat text to the widget. */
	UFUNCTION(BlueprintCallable, Category = "CombatText")
	void PushCombatText(const FGameplayTagContainer& StyleTags, const FText& Text);
	
	virtual void SetWidget(UUserWidget* Widget) override;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
protected:

};
