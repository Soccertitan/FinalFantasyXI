// Copyright Soccertitan 2025


#include "Input/InputActionTagMap.h"

const UInputAction* UInputActionTagMap::GetInputAction(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FInputMap& Input : Inputs)
	{
		if (Input.Action && Input.Tag == InputTag)
		{
			return Input.Action;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find InputAction for InputTag [%s], on CrysInputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
