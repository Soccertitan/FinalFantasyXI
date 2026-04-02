// Copyright Soccertitan 2025


#include "UI/ViewModel/OverlayViewModel.h"

#include "CrimAbilitySystemComponent.h"
#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "TargetingSystemBlueprintFunctionLibrary.h"
#include "TargetingSystemComponent.h"
#include "Player/CrysPlayerController.h"
#include "Player/CrysPlayerState.h"
#include "UI/ViewModel/TargetPointViewModel.h"


void UOverlayViewModel::OnInitializeViewModel(APlayerController* PlayerController)
{
	Super::OnInitializeViewModel(PlayerController);

	AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(PlayerController->GetPlayerState<ACrysPlayerState>());
	TargetingSystemComponent = UTargetingSystemBlueprintFunctionLibrary::GetTargetingSystemComponent(PlayerController);
	
	if (TargetingSystemComponent)
	{
		OnTargetPointUpdated(TargetingSystemComponent->GetTargetedPoint());
		TargetingSystemComponent->OnTargetedPointUpdatedDelegate.AddUniqueDynamic(this, &UOverlayViewModel::OnTargetPointUpdated);
	}
	
	if (ACrysPlayerController* CrysPC = Cast<ACrysPlayerController>(PlayerController))
	{
		CrysPC->OnRootWidgetAddedDelegate.AddUObject(this, &UOverlayViewModel::OnRootWidgetAdded);
		CrysPC->OnRootWidgetRemovedDelegate.AddUObject(this, &UOverlayViewModel::OnRootWidgetRemoved);
	}
}

void UOverlayViewModel::OnTargetPointUpdated(UTargetPointComponent* NewTarget)
{
	if (NewTarget)
	{
		K2_OnTargetPointSet(NewTarget);
	}
	else
	{
		K2_OnTargetPointRemoved();
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(OnTargetCleared);
	}
}

void UOverlayViewModel::OnRootWidgetAdded()
{
	SetRootWidgetAdded(true);
}

void UOverlayViewModel::OnRootWidgetRemoved()
{
	SetRootWidgetAdded(false);
}

void UOverlayViewModel::SetRootWidgetAdded(bool Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(bRootWidgetAdded, Value);
}
