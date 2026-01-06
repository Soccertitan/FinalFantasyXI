// Copyright Soccertitan 2025


#include "UI/ViewModel/Component/AttributeFractionViewModel.h"

#include "CrysLogChannels.h"
#include "UI/ViewModel/Component/AttributeViewModel.h"


UAttributeFractionViewModel::UAttributeFractionViewModel()
{
}

void UAttributeFractionViewModel::SetAttributesWithASC(UAbilitySystemComponent* InAbilitySystemComponent,
                                                    const FGameplayTag NumeratorAttributeTag, const FGameplayTag DenominatorAttributeTag)
{
	if (!InAbilitySystemComponent)
	{
		UE_LOG(LogCrys, Error, TEXT("Invalid AbilitySystemComponent in [%s]"), *GetName());
		return;
	}

	CreateViewModelsAndBindToDelegates();

	NumeratorAttribute->SetAttribute(NumeratorAttributeTag, InAbilitySystemComponent);
	DenominatorAttribute->SetAttribute(DenominatorAttributeTag, InAbilitySystemComponent);
}

void UAttributeFractionViewModel::SetAttributes(const FGameplayTag NumeratorAttributeTag,
	float NumeratorCurrentValue, float NumeratorBaseValue, const FGameplayTag DenominatorAttributeTag,
	float DenominatorCurrentValue, float DenominatorBaseValue)
{
	CreateViewModelsAndBindToDelegates();

	NumeratorAttribute->SetAttribute(NumeratorAttributeTag, NumeratorCurrentValue, NumeratorBaseValue);
	DenominatorAttribute->SetAttribute(DenominatorAttributeTag, DenominatorCurrentValue, DenominatorBaseValue);
}

float UAttributeFractionViewModel::GetPercentCurrentValue() const
{
	if (!NumeratorAttribute || !DenominatorAttribute)
	{
		return 0.f;
	}

	if (FMath::IsNearlyZero(DenominatorAttribute->GetCurrentValue()))
	{
		return 0.f;
	}
	return NumeratorAttribute->GetCurrentValue() / DenominatorAttribute->GetCurrentValue();
}

void UAttributeFractionViewModel::CreateViewModelsAndBindToDelegates()
{
	if (!NumeratorAttribute)
	{
		NumeratorAttribute = NewObject<UAttributeViewModel>(this, UAttributeViewModel::StaticClass());
		NumeratorAttribute->OnCurrentValueChangedDelegate.AddUObject(this, &UAttributeFractionViewModel::BroadcastValueChanged);
	}
	if (!DenominatorAttribute)
	{
		DenominatorAttribute = NewObject<UAttributeViewModel>(this, UAttributeViewModel::StaticClass());
		DenominatorAttribute->OnCurrentValueChangedDelegate.AddUObject(this, &UAttributeFractionViewModel::BroadcastValueChanged);
	}
}

void UAttributeFractionViewModel::BroadcastValueChanged()
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPercentCurrentValue);
}
