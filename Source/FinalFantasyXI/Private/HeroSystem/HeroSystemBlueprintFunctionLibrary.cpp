// Copyright Soccertitan 2025


#include "HeroSystem/HeroSystemBlueprintFunctionLibrary.h"

#include "HeroSystem/HeroManagerComponent.h"
#include "HeroSystem/HeroSystemInterface.h"

UHeroManagerComponent* UHeroSystemBlueprintFunctionLibrary::GetHeroManagerComponent(const AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return nullptr;
	}

	if (Actor->Implements<UHeroSystemInterface>())
	{
		return IHeroSystemInterface::Execute_GetHeroManagerComponent(Actor);
	}

	return Actor->FindComponentByClass<UHeroManagerComponent>();
}
