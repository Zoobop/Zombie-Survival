// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemodes/SurvivalGameState.h"
#include "Gamemodes/SurvivalMode.h"
#include "Entities/EntityState.h"
#include "Entities/Undead.h"
#include "Entities/Soldier.h"
#include "Spawning/UndeadSpawnPoint.h"
#include "Net/UnrealNetwork.h"
#include "Gameplay/DebrisBuys/Debris.h"
#include "StatAttributes/StatAttributeModifier.h"
#include "Entities/EntityStatComponent.h"

void ASurvivalGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASurvivalGameState, RoundNumber);
	DOREPLIFETIME(ASurvivalGameState, UndeadPerRound);
	DOREPLIFETIME(ASurvivalGameState, UndeadLeft);
	DOREPLIFETIME(ASurvivalGameState, UndeadModifiers);
}

void ASurvivalGameState::OnRep_RoundChanged()
{
	OnRoundChanged(UndeadPerRound);
	InitializeSpawning();
}

void ASurvivalGameState::StartNextRound_Implementation()
{
	/** Get number of undead to spawn this round */
	CalculateUndeadPerRound();
	/** Get the correct stats based on the round */
	CalculateUndeadStats();

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

		if (HasAuthority()) {
			OnUndeadKilled();
		}

		/** Check if killed undead was last undead alive */
		if (CurrentUndead.Num() == 0) {
			StartNextRound();
		}

	}
}

void ASurvivalGameState::InitializeSpawning()
{
	GetWorldTimerManager().SetTimer(TimerHandle_GameTimer, this, &ASurvivalGameState::BeginSpawnUndead, TimeBetweenSpawning, true, 2.0f);
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

void ASurvivalGameState::AddSpawnedUndead(class AUndead* Undead)
{
	/** Validate undead */
	if (Undead) {
		if (HasAuthority()) {

			if (RoundNumber > 1)
				Undead->GetEntityStatComponent()->ReceiveStatAttributeModification(UndeadModifiers);

			EState MovementState = EState::STATE_SLOW_WALK;

			if (RoundNumber >= 4 && RoundNumber <= 7) {
				MovementState = (EState)FMath::RandRange(1, 2);
			}
			else if (RoundNumber >= 7 && RoundNumber <= 12) {
				MovementState = (EState)FMath::RandRange(2, 3);
			}
			else {
				MovementState = EState::STATE_RUN;
			}

			Undead->SetMovementState(MovementState);
			OnUndeadSpawned(Undead);
		}
		CurrentUndead.Add(Undead);
	}
}

void ASurvivalGameState::CalculateUndeadPerRound()
{
	/** Calculated base */
	int32 NumberOfPlayers = 0;

	/** Get game mode */
	if (ASurvivalMode* SurvivalMode = Cast<ASurvivalMode>(GetWorld()->GetAuthGameMode())) {
		NumberOfPlayers = SurvivalMode->GetAlivePlayers().Num();
	}

	int32 CalculatedPlayerBase = DefaultUndeadAmount;
	if (NumberOfPlayers > 1)
		CalculatedPlayerBase = DefaultUndeadAmount + NumberOfPlayers - 1;

	int32 AlgorithmResult = CalculatedPlayerBase + RoundNumber * FMath::Pow((float)(DefaultUndeadAmount + NumberOfPlayers), 0.42f);

	UndeadPerRound = AlgorithmResult;
	UndeadLeft = UndeadPerRound;

}

void ASurvivalGameState::CalculateUndeadStats()
{
	/** Calculated base */
	int32 CalculatedHealth = (DefaultUndeadHealth * 0.5f) + ((RoundNumber * (DefaultUndeadHealth * .23f)));
	int32 CalculatedSpeed = (DefaultUndeadSpeed * 0.5f) + ((RoundNumber * (DefaultUndeadHealth * .09f)));

	UStatAttributeModifier* HealthModifier = UStatAttributeModifier::CreateModifier(CalculatedHealth, EModificationType::MODTYPE_INSTANT_INFINTE, EOperationType::OPTYPE_ADD, "Health");
	UStatAttributeModifier* SpeedModifier = UStatAttributeModifier::CreateModifier(CalculatedSpeed, EModificationType::MODTYPE_INSTANT_INFINTE, EOperationType::OPTYPE_ADD, "Speed");

	UndeadModifiers.Empty();
	UndeadModifiers.Append({ HealthModifier, SpeedModifier });
}

void ASurvivalGameState::InitializeRound()
{
	if (HasAuthority()) {
		/** Increment round number */
		++RoundNumber;
		OnRep_RoundChanged();
	}
}

void ASurvivalGameState::BeginSpawnUndead()
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
			UndeadSpawnLocations[RandomChannel][RandomSpawnLocation]->SpawnUndead();
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
