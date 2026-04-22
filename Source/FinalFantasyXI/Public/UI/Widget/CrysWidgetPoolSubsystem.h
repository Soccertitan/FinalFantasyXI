// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidgetPool.h"
#include "Subsystems/WorldSubsystem.h"
#include "CrysWidgetPoolSubsystem.generated.h"

/**
 * A simple subsystem for pooling widgets that are used across different actors within the world. I.E Damage number popups,
 * character names, etc.
 */
UCLASS()
class FINALFANTASYXI_API UCrysWidgetPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void PreDeinitialize() override;
	
	/**
	 * Gets an instance of a widget of the given class.
	 */
	UFUNCTION(BlueprintCallable, Category = "Subsystem", meta = (DeterminesOutputType = "WidgetClass"))
	UUserWidget* GetOrCreateInstance(TSubclassOf<UUserWidget> WidgetClass);
	
	/** Return a widget object to the pool, allowing it to be reused in the future */
	UFUNCTION(BlueprintCallable, Category = "Subsystem")
	void Release(TArray<UUserWidget*> Widgets, bool bReleaseSlate = false);
	
private:
	
	UPROPERTY(Transient)
	FUserWidgetPool WidgetPool;
};
