// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Engine/StreamableManager.h"
#include "GameplayAbilityDefinitionViewModel.generated.h"

class UGameplayAbilityDefinition;

/**
 * Display information from a CrysGameplayAbilityDefinition.
 */
UCLASS()
class FINALFANTASYXI_API UGameplayAbilityDefinitionViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	/** Loads the GameplayAbilityDefinition from the CrimGameplayAbility. */
	void LoadAbilityDefinition(TSoftObjectPtr<UGameplayAbilityDefinition> InGameplayAbilityDefinition);
	/** Loads the ability definition if it hasn't already. */
	void SetAbilityDefinition(UGameplayAbilityDefinition* InGameplayAbilityDefinition);

	FText GetAbilityName() const { return AbilityName; }
	UTexture2D* GetIcon() const { return Icon; }

	/** Clears out all data. */
	void ResetData();

protected:
	void SetAbilityName(const FText InValue);
	void SetIcon(UTexture2D* InValue);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	FText AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY()
	TObjectPtr<UGameplayAbilityDefinition> GameplayAbilityDefinition;

	TSharedPtr<FStreamableHandle> StreamableHandleAbilityDefinition;
	void Internal_LoadAbilityDefinition(FPrimaryAssetId PrimaryAssetId);
	void OnAbilityDefinitionLoaded(FPrimaryAssetId LoadedAssetId);
};
