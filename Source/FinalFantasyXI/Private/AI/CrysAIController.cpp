// Copyright Soccertitan 2025


#include "AI/CrysAIController.h"


ACrysAIController::ACrysAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACrysAIController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (IGenericTeamAgentInterface* Interface = Cast<IGenericTeamAgentInterface>(GetPawn()))
	{
		Interface->SetGenericTeamId(NewTeamID);
	}
}

FGenericTeamId ACrysAIController::GetGenericTeamId() const
{
	if (IGenericTeamAgentInterface* Interface = Cast<IGenericTeamAgentInterface>(GetPawn()))
	{
		return Interface->GetGenericTeamId();
	}
	return FGenericTeamId();
}

