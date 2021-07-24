// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemodes/SurvivalGameState.h"
#include "Net/UnrealNetwork.h"

void ASurvivalGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASurvivalGameState, RoundNumber);
}

void ASurvivalGameState::OnRep_RoundChanged()
{
	
}

void ASurvivalGameState::StartNextRound()
{
	/** Get number of undead to spawn this round */
	CalculateUndeadPerRound();

	/** Do round stuff... */
	GetWorldTimerManager().SetTimer(TimerHandle_RoundCountDown, this, &ASurvivalGameState::InitializeRound, 10.0f, false);
}

void ASurvivalGameState::GameOver()
{

}

void ASurvivalGameState::CalculateUndeadPerRound()
{

}

void ASurvivalGameState::InitializeRound()
{
	/** Increment round number */
	RoundNumber++;
}
