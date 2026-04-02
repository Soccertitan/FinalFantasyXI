// Copyright Soccertitan 2025


#include "UI/ViewModel/TargetPointViewModel.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "TargetPointComponent.h"
#include "Character/CrysCharacter.h"

void UTargetPointViewModel::SetTargetPoint(UTargetPointComponent* TargetPoint)
{
	if (!TargetPoint)
	{
		return;
	}
	
	if (ACrysCharacter* Character = Cast<ACrysCharacter>(TargetPoint->GetOwner()))
	{
		SetTargetName(Character->GetCharacterName());
	}
	
	AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(TargetPoint->GetOwner());
	K2_SetTargetPoint(TargetPoint);
}

void UTargetPointViewModel::SetTargetName(FText Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(TargetName, Value);
}
