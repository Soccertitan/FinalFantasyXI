// Copyright Soccertitan 2025


#include "Player/CrysPlayerController.h"

#include "CrimEnhancedInputComponent.h"
#include "CrysGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "UINavPCComponent.h"
#include "UINavWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Input/InputActionListenerMap.h"
#include "UI/HUD/CrysHUD.h"

ACrysPlayerController::ACrysPlayerController()
{
	UINavPCComponent = CreateDefaultSubobject<UUINavPCComponent>(TEXT("UINav PC Component"));
	UINavPCComponent->AutoHideMouse = EAutoHideMouse::Never;

	bReplicates = true;
}

void ACrysPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	ApplyDefaultInputActionMap();
	EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	
	for (const auto& Pair : InputMappingContextMap)
	{
		if (Pair.Key)
		{
			EnhancedInputSubsystem->AddMappingContext(Pair.Key, Pair.Value);
		}
	}
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
		UINavPCComponent->GetActiveWidget()->ReturnToParent(true);
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

	EnhancedInputSubsystem->AddTagToInputMode(FCrysGameplayTags::Get().EnhancedInput_Modes_UI);

	if (bRestrictMovement)
	{
		SetIgnoreLookInput(true);
		SetIgnoreMoveInput(true);
	}
}

void ACrysPlayerController::OnRootWidgetRemoved_Implementation()
{
	IUINavPCReceiver::OnRootWidgetRemoved_Implementation();
	
	EnhancedInputSubsystem->RemoveTagFromInputMode(FCrysGameplayTags::Get().EnhancedInput_Modes_UI);

	SetIgnoreLookInput(false);
	SetIgnoreMoveInput(false);
	
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
}

void ACrysPlayerController::ApplyDefaultInputActionMap()
{
	UCrimEnhancedInputComponent* EnhancedInputComponent = Cast<UCrimEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		for (const TObjectPtr<UInputActionListenerMap>& InputActionListenerMap : DefaultInputActionListeners)
		{
			if (InputActionListenerMap)
			{
				for (const TTuple<TObjectPtr<UInputAction>, TSubclassOf<UInputActionListener>>& Pair : InputActionListenerMap->InputActionMap)
				{
					EnhancedInputComponent->SetListener(Pair.Key, Pair.Value);
				}
			}
		}
	}
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
