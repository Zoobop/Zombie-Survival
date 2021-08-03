
// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Undead.h"
#include "Entities/EntityStatComponent.h"
#include "Entities/Soldier.h"
#include "Controllers/EntityController.h"
#include "Gamemodes/SurvivalGameState.h"
#include "Gamemodes/SurvivalMode.h"

AUndead::AUndead()
{
	GetEntityStatComponent()->UndeadDefaults();
}

void AUndead::SetMovementState(EState MovementState)
{
	State = MovementState;
}

void AUndead::SendDeathData(AEntity* Killed)
{
	if (HasAuthority()) {
		if (ASoldier* Player = Cast<ASoldier>(Killed)) {
			if (ASurvivalMode* GameMode = Cast<ASurvivalMode>(GetWorld()->GetAuthGameMode())) {
				GameMode->PlayerDied(Player);
			}
		}
	}
}

TArray<class UStatAttributeModifier*> AUndead::ApplyStatAttributeModification()
{
	TArray<class UStatAttributeModifier*> Modifiers;

	Modifiers.Add(StatAttributeModifier);

	return Modifiers;
}

void AUndead::BeginPlay()
{
	Super::BeginPlay();

	ValidateUndead();
}

void AUndead::ValidateUndead()
{
	/** Get the game state */
	if (ASurvivalGameState* GameState = Cast<ASurvivalGameState>(GetWorld()->GetGameState())) {
		GameState->AddSpawnedUndead(this);

		GetEntityStatComponent()->ReConfigureAttributes();

		InitializeSearch();
	}
}

void AUndead::InitializeSearch_Implementation()
{
	/** Start search timer */
	if (!TimerHandle_SearchTimer.IsValid()) {
		GetWorldTimerManager().SetTimer(TimerHandle_SearchTimer, this, &AUndead::SearchForClosestPlayer, SearchInterval, true);
	}
}

void AUndead::StopSearch_Implementation()
{
	/** Stop search timer */
	if (TimerHandle_SearchTimer.IsValid()) {
		GetWorldTimerManager().ClearTimer(TimerHandle_SearchTimer);
	}
}

void AUndead::SearchForClosestPlayer()
{
	/** Get game mode */
	if (HasAuthority()) {
		if (ASurvivalMode* SurvivalMode = Cast<ASurvivalMode>(GetWorld()->GetAuthGameMode())) {

			/** Find and move to the closest player */
			FindPlayer(SurvivalMode);
		}
	}
}

