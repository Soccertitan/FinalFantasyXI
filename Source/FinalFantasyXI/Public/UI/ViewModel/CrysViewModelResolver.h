// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "View/MVVMViewModelContextResolver.h"
#include "CrysViewModelResolver.generated.h"

class UCrysViewModel;
/**
 * Retrieves a ViewModel from the CrysHUD.
 */
UCLASS()
class FINALFANTASYXI_API UCrysViewModelResolver : public UMVVMViewModelContextResolver
{
	GENERATED_BODY()

public:
	virtual UObject* CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget, const UMVVMView* View) const override;
	virtual bool DoesSupportViewModelClass(const UClass* Class) const override;

protected:
	UCrysViewModel* GetViewModel(const UClass* ExpectedType, const UUserWidget* UserWidget) const;
};
