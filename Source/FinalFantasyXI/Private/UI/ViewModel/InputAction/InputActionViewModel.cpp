// Copyright Soccertitan 2025


#include "UI/ViewModel/InputAction/InputActionViewModel.h"

#include "CrimEnhancedInputComponent.h"
#include "CrysLogChannels.h"
#include "Input/InputAction/CrysInputActionListener.h"
#include "UI/ViewModel/InputAction/InputActionListenerViewModel.h"

UInputActionListenerViewModel* UInputActionViewModel::FindOrCreateInputActionListenerViewModel(UInputAction* InputAction)
{
	if (InputAction)
	{
		for (UInputActionListenerViewModel* ViewModel : InputActionListenerViewModels)
		{
			if (ViewModel->GetInputAction() == InputAction)
			{
				return ViewModel;
			}
		}
		return InternalCreateInputActionListenerViewModel(InputAction);
	}
	return nullptr;
}

void UInputActionViewModel::OnInitializeViewModel(APlayerController* PlayerController)
{
	Super::OnInitializeViewModel(PlayerController);
	
	InitEnhancedInputComponent(PlayerController);
}

void UInputActionViewModel::InitEnhancedInputComponent(APlayerController* PlayerController)
{
	if (EnhancedInputComponent)
	{
		return;
	}
	
	EnhancedInputComponent = Cast<UCrimEnhancedInputComponent>(PlayerController->InputComponent);
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogCrys, Error, TEXT("%s does not have a CrimEnhancedInputComponent"), *GetNameSafe(PlayerController));
		return;
	}
}

UInputActionListenerViewModel* UInputActionViewModel::InternalCreateInputActionListenerViewModel(UInputAction* InputAction)
{
	UInputActionListenerViewModel* NewVM = NewObject<UInputActionListenerViewModel>(this);
	UCrysInputActionListener* InputActionListener = nullptr;
	if (EnhancedInputComponent)
	{
		InputActionListener = Cast<UCrysInputActionListener>(EnhancedInputComponent->FindListener(InputAction));
	}
	NewVM->InitializeInputActionListener(InputAction, InputActionListener);
	InputActionListenerViewModels.Add(NewVM);
	return NewVM;
}
