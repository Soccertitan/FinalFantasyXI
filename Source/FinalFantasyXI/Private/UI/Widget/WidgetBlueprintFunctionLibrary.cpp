// Copyright Soccertitan 2025


#include "UI/Widget/WidgetBlueprintFunctionLibrary.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"

TArray<UWidget*> UWidgetBlueprintFunctionLibrary::TraverseWidgetHierarchy(UUserWidget* UserWidget, TSubclassOf<UUserWidget> TestClass)
{
	TArray<UWidget*> OutWidgets;

	if (IsValid(UserWidget) && IsValid(TestClass))
	{
		TArray<UWidget*> Widgets;
		UserWidget->WidgetTree->GetAllWidgets(Widgets);
		for (UWidget* Widget : Widgets)
		{
			if (Widget->IsA(TestClass))
			{
				OutWidgets.Add(Widget);
			}
		}
	}
	
	return OutWidgets;
}
