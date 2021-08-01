// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemodes/SurvivalGameState.h"
#include "Gamemodes/SurvivalMode.h"
#include "Entities/EntityState.h"
#include "Entities/Undead.h"
#include "Entities/Soldier.h"
#include "Spawning/UndeadSpawnPoint.h"
#include "Net/UnrealNetwork.h"
#include "Gameplay/DebrisBuys/Debris.h"

void ASurvivalGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASurvivalGameState, RoundNumber);
}

void ASurvivalGameState::OnRep_RoundChanged()
{
	OnRoundChanged();
	InitializeSpawning();
}

void ASurvivalGameState::StartNextRound()
{
	/** Get number of undead to spawn this round */
	CalculateUndeadPerRound();

	/** Do round stuff... */
	GetWorldTimerManager().SetTimer(TimerHandle_GameTimer, this, &ASurvivalGameState::InitializeRound, TimeBetweenRounds, false);
}

void ASurvivalGameState::GameOver()
{

}

void ASurvivalGameState::UndeadKilled(class AUndead* Killed, class ASoldier* Killer)
{
	if (Killed) {

		/** Remove killed undead from list of current undead */
		CurrentUndead.RemoveSingle(Killed);

		/** Award the player with points */
		if (Killer) {
			if (AEntityState* EntityState = Cast<AEntityState>(Killer->GetPlayerState())) {
				EntityState->AddPoints(AEntityState::PointsPerKill);
				EntityState->IncrementKillTotal();
			}
		}

		/** Check if killed undead was last undead alive */
		if (CurrentUndead.Num() == 0) {
			StartNextRound();
		}

	}
}

void ASurvivalGameState::InitializeSpawning()
{
	GetWorldTimerManager().SetTimer(TimerHandle_GameTimer, this, &ASurvivalGameState::SpawnUndead, TimeBetweenSpawning, true, 2.0f);
}

void ASurvivalGameState::AddSpawnLocation(class AUndeadSpawnPoint* SpawnPoint)
{
	/** Add spawn point to the list */
	if (SpawnPoint) {

		/** Check if spawn point tag is already a key */
		if (UndeadSpawnLocations.Contains(SpawnPoint->GetDebrisChannel())) {
			auto Spawnpoints = UndeadSpawnLocations.Find(SpawnPoint->GetDebrisChannel());
			Spawnpoints->Add(SpawnPoint);
		}
		else {
			UndeadSpawnLocations.Add(SpawnPoint->GetDebrisChannel(), { SpawnPoint });
		}

		OnSpawnLocationAdded(SpawnPoint);
	}
}

void ASurvivalGameState::AddDebrisChannel(class ADebris* Debris)
{
	/** Add debris to the list */
	if (Debris) {
		MapDebris.Add(Debris);
	}
}

void ASurvivalGameState::CalculateUndeadPerRound()
{

}

void ASurvivalGameState::InitializeRound()
{
	/** Increment round number */
	++RoundNumber;
}

void ASurvivalGameState::SpawnUndead()
{
	/** Check if there is room to spawn */
	if (CanSpawn()) {
		/** Validate lists */
		if (UndeadSpawnLocations.Num() > 0) {

			/** Get number of keys */
			TSet<int32> KeySet;
			UndeadSpawnLocations.GetKeys(KeySet);

			/** Get random map debris channel */
			int32 RandomChannel = FMath::RandRange(0, KeySet.Num() - 1);
			int32 RandomSpawnLocation = FMath::RandRange(0, UndeadSpawnLocations[RandomChannel].Num() - 1);

			/** Spawn undead at location */
			FVector SpawnLocation = UndeadSpawnLocations[RandomChannel][RandomSpawnLocation]->FindSpawnLocation();
			FRotator SpawnRotation = FRotator::ZeroRotator;
			FActorSpawnParameters SpawnParams;

			AUndead* SpawnedUndead = GetWorld()->SpawnActor<AUndead>(AUndead::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

			CurrentUndead.Add(SpawnedUndead);
		}
	}
	else {
		ClearSpawnTimer();
	}
}

void ASurvivalGameState::ClearSpawnTimer()
{
	if (TimerHandle_GameTimer.IsValid()) {
		GetWorldTimerManager().ClearTimer(TimerHandle_GameTimer);
	}
}
