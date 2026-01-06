// Copyright Soccertitan 2025


#include "UI/ViewModel/Component/GameplayAbilityDefinitionViewModel.h"

#include "AbilitySystem/CrysGameplayAbilityDefinition.h"
#include "Engine/AssetManager.h"


void UGameplayAbilityDefinitionViewModel::LoadAbilityDefinition(TSoftObjectPtr<UGameplayAbilityDefinition> InGameplayAbilityDefinition)
{
	if (InGameplayAbilityDefinition.IsNull())
	{
		ResetData();
	}
	else if (GameplayAbilityDefinition != InGameplayAbilityDefinition)
	{
		FPrimaryAssetId PrimaryAssetId = InGameplayAbilityDefinition->GetPrimaryAssetId();
		Internal_LoadAbilityDefinition(PrimaryAssetId);
	}
}

void UGameplayAbilityDefinitionViewModel::SetAbilityDefinition(UGameplayAbilityDefinition* InGameplayAbilityDefinition)
{
	if (!InGameplayAbilityDefinition)
	{
		ResetData();
	}
	else if (GameplayAbilityDefinition != InGameplayAbilityDefinition)
	{
		GameplayAbilityDefinition = InGameplayAbilityDefinition;
		Internal_LoadAbilityDefinition(GameplayAbilityDefinition->GetPrimaryAssetId());
	}
}

void UGameplayAbilityDefinitionViewModel::SetAbilityName(const FText InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(AbilityName, InValue);
}

void UGameplayAbilityDefinitionViewModel::SetIcon(UTexture2D* InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(Icon, InValue);
}

void UGameplayAbilityDefinitionViewModel::ResetData()
{
	SetAbilityName(FText());
	SetIcon(nullptr);
	GameplayAbilityDefinition = nullptr;
}

void UGameplayAbilityDefinitionViewModel::Internal_LoadAbilityDefinition(FPrimaryAssetId PrimaryAssetId)
{
	ResetData();
	StreamableHandleAbilityDefinition.Reset();
	if (PrimaryAssetId.IsValid())
	{
		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &UGameplayAbilityDefinitionViewModel::OnAbilityDefinitionLoaded, PrimaryAssetId);
		StreamableHandleAbilityDefinition = UAssetManager::Get().PreloadPrimaryAssets({PrimaryAssetId}, {"UI"}, false, Delegate);
	}
}

void UGameplayAbilityDefinitionViewModel::OnAbilityDefinitionLoaded(FPrimaryAssetId LoadedAssetId)
{
	GameplayAbilityDefinition = Cast<UGameplayAbilityDefinition>(UAssetManager::Get().GetPrimaryAssetObject(LoadedAssetId));
	if (GameplayAbilityDefinition)
	{
		SetAbilityName(GameplayAbilityDefinition->AbilityName);
		SetIcon(GameplayAbilityDefinition->Icon.Get());
	}
}
