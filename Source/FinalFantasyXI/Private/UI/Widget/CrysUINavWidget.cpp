// Copyright Soccertitan 2025


#include "UI/Widget/CrysUINavWidget.h"

#include "UINavPCComponent.h"
#include "UINavSettings.h"

void UCrysUINavWidget::SetReturnToWidgetMap(UCrysUINavWidget* Key, UCrysUINavWidget* Value)
{
	if (IsValid(Key))
	{
		ReturnToWidgetMap.Add(Key, Value);
	}
}

void UCrysUINavWidget::RemoveReturnToWidgetMap(UCrysUINavWidget* Key)
{
	ReturnToWidgetMap.Remove(Key);
}

void UCrysUINavWidget::ClearReturnToWidgetMap()
{
	ReturnToWidgetMap.Empty();
}

void UCrysUINavWidget::ReturnToWidgetInMap()
{
	OnReturn();
}

void UCrysUINavWidget::UINavOnReturn_Implementation()
{
	if(GetDefault<UUINavSettings>()->bRemoveWidgetOnReturn)
	{
		ReturnToParent();
	}
}

void UCrysUINavWidget::OnReturn_Implementation()
{
	if (ReturnToWidgetMap.IsEmpty() != true)
	{
		UCrysUINavWidget* PreviousActiveWidget = Cast<UCrysUINavWidget>(UINavPC->GetActiveWidget());
		while (PreviousActiveWidget != nullptr)
		{
			if (TObjectPtr<UCrysUINavWidget>* Value = ReturnToWidgetMap.Find(PreviousActiveWidget))
			{
				UCrysUINavWidget* NewActiveWidget = *Value;
				NewActiveWidget->SetFocus();
				UINavOnReturnFromMap(PreviousActiveWidget, NewActiveWidget);
				NewActiveWidget->UINavOnReturnToWidget(PreviousActiveWidget);
				PreviousActiveWidget->UINavOnReturnFromWidget(NewActiveWidget);
				return;
			}
			PreviousActiveWidget = Cast<UCrysUINavWidget>(PreviousActiveWidget->OuterUINavWidget);
		}
	}

	UINavOnReturn();
}

void UCrysUINavWidget::UINavOnReturnToWidget(UCrysUINavWidget* PreviousActiveWidget)
{
	OnReturnToWidgetDelegate.Broadcast(PreviousActiveWidget);
	K2_UINavOnReturnToWidget(PreviousActiveWidget);
}

void UCrysUINavWidget::UINavOnReturnFromWidget(UCrysUINavWidget* NewActiveWidget)
{
	OnReturnFromWidgetDelegate.Broadcast(NewActiveWidget);
	K2_UINavOnReturnFromWidget(NewActiveWidget);
}
