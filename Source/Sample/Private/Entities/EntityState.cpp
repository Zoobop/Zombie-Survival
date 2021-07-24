// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/EntityState.h"

AEntityState::AEntityState()
{

}

void AEntityState::AddPoints(int32 Points)
{
	TotalPoints += Points;
	CurrentPoints += Points;
}

void AEntityState::RemovePoints(int32 Points)
{
	CurrentPoints -= Points;
}

void AEntityState::IncrementKillTotal()
{
	Kills++;
}
