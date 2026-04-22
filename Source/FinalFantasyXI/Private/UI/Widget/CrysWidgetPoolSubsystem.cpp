// Copyright Soccertitan 2025


#include "UI/Widget/CrysWidgetPoolSubsystem.h"


void UCrysWidgetPoolSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	WidgetPool.SetWorld(GetWorld());
}

void UCrysWidgetPoolSubsystem::PreDeinitialize()
{
	Super::PreDeinitialize();
	
	WidgetPool.ResetPool();
}

UUserWidget* UCrysWidgetPoolSubsystem::GetOrCreateInstance(TSubclassOf<UUserWidget> WidgetClass)
{
	return WidgetPool.GetOrCreateInstance(WidgetClass);
}

void UCrysWidgetPoolSubsystem::Release(TArray<UUserWidget*> Widgets, bool bReleaseSlate)
{
	WidgetPool.Release(Widgets, bReleaseSlate);
}
