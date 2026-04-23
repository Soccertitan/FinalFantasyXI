// Copyright Soccertitan 2025


#include "Character/CombatTextComponent.h"

#include "CrysLogChannels.h"
#include "Misc/DataValidation.h"
#include "UI/Widget/CombatTextContainerWidget.h"


UCombatTextComponent::UCombatTextComponent()
{
	SetWidgetSpace(EWidgetSpace::Screen);
	bDrawAtDesiredSize = true;
	SetCollisionProfileName(FName("NoCollision"));
}

void UCombatTextComponent::PushCombatText(const FGameplayTagContainer& StyleTags, const FText& Text)
{
	if (UCombatTextContainerWidget* CombatTextWidget = Cast<UCombatTextContainerWidget>(GetWidget()))
	{
		CombatTextWidget->PushCombatText(StyleTags, Text);
	}
}

void UCombatTextComponent::SetWidget(UUserWidget* InWidget)
{
	if (UCombatTextContainerWidget* CombatTextWidget = Cast<UCombatTextContainerWidget>(InWidget))
	{
		Super::SetWidget(InWidget);
	}
	else
	{
		UE_LOG(LogCrys, Error, TEXT("%s is not of type CombatTextContainerWidget in %s"), *GetNameSafe(InWidget), *GetOwner()->GetName());
	}
}

EDataValidationResult UCombatTextComponent::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	if (WidgetClass && !WidgetClass->IsChildOf(UCombatTextContainerWidget::StaticClass()))
	{
		Context.AddError(FText::FromString("WidgetClass is not of type CombatTextContainerWidget"));
		Result = EDataValidationResult::Invalid;
	}
	return Result;
}

