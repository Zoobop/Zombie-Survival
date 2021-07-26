// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/EntityState.h"
#include "Gamemodes/SurvivalGameState.h"

AEntityState::AEntityState()
{

}

void AEntityState::AddPoints(int32 Points)
{
	float Multiplier = 1.0f;
	if (ASurvivalGameState* State = Cast<ASurvivalGameState>(GetWorld()->GetGameState())) {
		if (State->IsDoublePointsActive())
			Multiplier = 2.0f;
	}

	TotalPoints += Points * Multiplier;
	CurrentPoints += Points * Multiplier;
}

void AEntityState::RemovePoints(int32 Points)
{
	CurrentPoints -= Points;
}

void AEntityState::IncrementKillTotal()
{
	Kills++;
}
