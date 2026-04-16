// Copyright Soccertitan 2025


#include "UI/ViewModel/OverlayViewModel.h"

#include "CrimAbilitySystemComponent.h"
#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "TargetingSystemBlueprintFunctionLibrary.h"
#include "TargetingSystemComponent.h"
#include "AbilitySystem/Ability/Combat/AutoAttackManagerComponent.h"
#include "Player/CrysPlayerController.h"
#include "Player/CrysPlayerState.h"
#include "UI/ViewModel/TargetPointViewModel.h"


void UOverlayViewModel::InitializeViewModel(APlayerController* PlayerController)
{
	Super::InitializeViewModel(PlayerController);

	if (UCrimAbilitySystemComponent* AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(PlayerController->GetPlayerState<ACrysPlayerState>()))
	{
		OnAbilitySystemComponentRetrieved(AbilitySystemComponent);
	}
	
	if (ACrysPlayerController* CrysPC = Cast<ACrysPlayerController>(PlayerController))
	{
		CrysPC->OnRootWidgetAddedDelegate.AddUObject(this, &UOverlayViewModel::OnRootWidgetAdded);
		CrysPC->OnRootWidgetRemovedDelegate.AddUObject(this, &UOverlayViewModel::OnRootWidgetRemoved);
	}
	
	AutoAttackManagerComponent = PlayerController->GetPlayerState<ACrysPlayerState>()->FindComponentByClass<UAutoAttackManagerComponent>();
	if (AutoAttackManagerComponent)
	{
		AutoAttackManagerComponent->OnAutoAttackStateChangedDelegate.AddUniqueDynamic(this, &UOverlayViewModel::SetIsAutoAttacking);
		SetIsAutoAttacking(AutoAttackManagerComponent->IsAutoAttacking());
	}
}

void UOverlayViewModel::SetIsAutoAttacking(bool Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(bAutoAttacking, Value);
}

void UOverlayViewModel::OnRootWidgetAdded()
{
	SetIsRootWidgetAdded(true);
}

void UOverlayViewModel::OnRootWidgetRemoved()
{
	SetIsRootWidgetAdded(false);
}

void UOverlayViewModel::SetIsRootWidgetAdded(bool Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(bRootWidgetAdded, Value);
}

void UOverlayViewModel::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	Super::OnPossessedPawnChanged(OldPawn, NewPawn);
	
	InitTargetingSystemComponent(NewPawn);
}

void UOverlayViewModel::InitTargetingSystemComponent(APawn* NewPawn)
{
	if (TargetingSystemComponent)
	{
		TargetingSystemComponent->OnTargetedPointUpdatedDelegate.RemoveAll(this);
	}
	
	TargetingSystemComponent = UTargetingSystemBlueprintFunctionLibrary::GetTargetingSystemComponent(NewPawn);
	
	if (TargetingSystemComponent)
	{
		OnTargetPointUpdated(TargetingSystemComponent->GetTargetedPoint());
		TargetingSystemComponent->OnTargetedPointUpdatedDelegate.AddUniqueDynamic(this, &UOverlayViewModel::OnTargetPointUpdated);
	}
}
