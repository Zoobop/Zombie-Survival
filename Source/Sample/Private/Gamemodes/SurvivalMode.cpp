// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemodes/SurvivalMode.h"
#include "Gamemodes/SurvivalGameState.h"
#include "Entities/EntityState.h"
#include "Controllers/EntityController.h"
#include "Entities/Undead.h"
#include "Entities/Soldier.h"
#include "UObject/ConstructorHelpers.h"
#include "Spawning/UndeadSpawnPoint.h"

ASurvivalMode::ASurvivalMode() : Super()
{
// 	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/ZombieSurvival/Characters/Blueprints/BP_Soldier"));
// 	DefaultPawnClass = PlayerPawnClassFinder.Class;

	
}

void ASurvivalMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AlivePlayers.Add(Cast<AEntityController>(NewPlayer));
}

void ASurvivalMode::PlayerDied(class ASoldier* Killed)
{
	if (Killed) {

		if (AEntityController* Player = Cast<AEntityController>(Killed)) {
			AlivePlayers.RemoveSingle(Player);
		}

		/** Check if killed player was last player alive */
		if (AlivePlayers.Num() == 0) {
			if (ASurvivalGameState* SurvivalGameState = GetGameState<ASurvivalGameState>()) {
				SurvivalGameState->GameOver();
			}
		}

	}
}

void ASurvivalMode::BeginPlay()
{
	Super::BeginPlay();

	if (ASurvivalGameState* SurvivalGameState = GetGameState<ASurvivalGameState>()) {
		SurvivalGameState->StartNextRound();
	}
}
