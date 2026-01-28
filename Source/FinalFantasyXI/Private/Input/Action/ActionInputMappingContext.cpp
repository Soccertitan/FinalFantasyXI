// Copyright Soccertitan 2025


#include "Input/Action/ActionInputMappingContext.h"

#include "EnhancedInputSubsystems.h"

void UActionInputMappingContext::OnInitializeAction()
{
	Super::OnInitializeAction();
	
	EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer());
	ensure(EnhancedInputSubsystem);
}

void UActionInputMappingContext::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	if (InputMappingContext && bAppliedContext == false)
	{
		EnhancedInputSubsystem->AddMappingContext(InputMappingContext, Priority, ContextOptions);
		bAppliedContext = true;
	}
}

void UActionInputMappingContext::OnInputActionCompleted(const FInputActionValue& Value)
{
	Super::OnInputActionCompleted(Value);

	EnhancedInputSubsystem->RemoveMappingContext(InputMappingContext, ContextOptions);
	bAppliedContext = false;
}

void UActionInputMappingContext::OnInputActionCanceled(const FInputActionValue& Value)
{
	Super::OnInputActionCanceled(Value);
	
	EnhancedInputSubsystem->RemoveMappingContext(InputMappingContext, ContextOptions);
	bAppliedContext = false;
}
