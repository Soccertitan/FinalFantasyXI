// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WidgetBlueprintFunctionLibrary.generated.h"

class UWidget;
/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UWidgetBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * @param UserWidget The widget we want to traverse through.
	 * @param TestClass The widget class we are searching for.
	 * @return All user widgets that are of the specified UserWidget Class.
	 */
	UFUNCTION(BlueprintCallable, Category = "WidgetFunctionLibrary", meta=(DeterminesOutputType=TestClass, DefaultToSelf = UserWidget))
	static TArray<UWidget*> TraverseWidgetHierarchy(UUserWidget* UserWidget, TSubclassOf<UUserWidget> TestClass);
};
