// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/CrysUserWidget.h"
#include "ActionWidget.generated.h"

class UActionViewModel;
/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UActionWidget : public UCrysUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetViewModel(UActionViewModel* ViewModel);
};
