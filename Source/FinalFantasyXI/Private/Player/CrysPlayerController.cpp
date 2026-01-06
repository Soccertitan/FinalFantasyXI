// Copyright Soccertitan 2025


#include "Player/CrysPlayerController.h"

#include "UINavPCComponent.h"
#include "UINavWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/HUD/CrysHUD.h"

ACrysPlayerController::ACrysPlayerController()
{
	UINavPCComponent = CreateDefaultSubobject<UUINavPCComponent>(TEXT("UINav PC Component"));
	UINavPCComponent->AutoHideMouse = EAutoHideMouse::Gamepad;

	bReplicates = true;
}

void ACrysPlayerController::CreateUINavWidgetAndAddToViewport(TSubclassOf<UUINavWidget> WidgetClass, bool bInRestrictMovement)
{
	if (WidgetClass)
	{
		UUINavWidget* NewWidget = CreateWidget<UUINavWidget>(this, WidgetClass);
		AddUINavWidgetToViewport(NewWidget, nullptr, bInRestrictMovement);
	}
}

void ACrysPlayerController::AddUINavWidgetToViewport(UUINavWidget* InWidget, UWidget* InWidgetToFocus, bool bInRestrictMovement)
{
	if (!IsLocalPlayerController())
	{
		return;
	}

	bRestrictMovement = bInRestrictMovement;
	
	if (UINavPCComponent->GetActiveWidget())
	{
		UINavPCComponent->GetActiveWidget()->RemoveSelfAndAllParents();
	}
	
	if (IsValid(InWidget))
	{
		InWidget->AddToPlayerScreen();
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, InWidgetToFocus);
	}
}

void ACrysPlayerController::OnRootWidgetAdded_Implementation()
{
	IUINavPCReceiver::OnRootWidgetAdded_Implementation();

	SetShowMouseCursor(true);

	if (bRestrictMovement)
	{
		SetIgnoreLookInput(true);
		SetIgnoreMoveInput(true);
	}
}

void ACrysPlayerController::OnRootWidgetRemoved_Implementation()
{
	IUINavPCReceiver::OnRootWidgetRemoved_Implementation();

	SetShowMouseCursor(false);
	SetIgnoreLookInput(false);
	SetIgnoreMoveInput(false);
}

void ACrysPlayerController::InitializeHUD()
{
	if (ACrysHUD* PSOHud = Cast<ACrysHUD>(GetHUD()))
	{
		if (bHudInitialized == false)
		{
			PSOHud->InitializeHUD();
			bHudInitialized = true;
		}
	}
}
