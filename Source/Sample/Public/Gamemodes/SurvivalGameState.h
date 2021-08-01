// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SurvivalGameState.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API ASurvivalGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void StartNextRound();

	UFUNCTION(BlueprintCallable)
	void GameOver();

	UFUNCTION(BlueprintCallable)
	void UndeadKilled(class AUndead* Undead, class ASoldier* Player);

	UFUNCTION()
	void InitializeSpawning();

	/** Register the spawn point */
	UFUNCTION()
	void AddSpawnLocation(class AUndeadSpawnPoint* SpawnPoint);

	/** Register the debris */
	UFUNCTION()
	void AddDebrisChannel(class ADebris* Debris);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSpawnLocationAdded(class AUndeadSpawnPoint* SpawnPoint);

	/** Returns if anymore undead can spawn */
	FORCEINLINE bool CanSpawn() const { return CurrentUndead.Num() < MaxSpawnedUndead && CurrentUndead.Num() < UndeadPerRound; }
	/** Returns if Double Points is activate */
	FORCEINLINE bool IsDoublePointsActive() const { return bDoublePointsActivated; }

protected:

	void CalculateUndeadPerRound();

	void InitializeRound();

	void SpawnUndead();

	void ClearSpawnTimer();

	UFUNCTION(BlueprintImplementableEvent)
	void OnRoundChanged();



	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_RoundChanged();

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = "OnRep_RoundChanged", Category = "SurvivalGame")
	int32 RoundNumber = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SurvivalGame")
	int32 UndeadPerRound = 6;

	/** Number of undead at a time in the map */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxSpawnedUndead = 50;

	/** Locations the undead can spawn */
	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<class ADebris*> MapDebris;

	/** Current undead in the game */
	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<class AUndead*> CurrentUndead;

private:

	/** Locations the undead can spawn */
	TMap<int32, TArray<class AUndeadSpawnPoint*>> UndeadSpawnLocations;

	UPROPERTY(EditDefaultsOnly, Category = "SurvivalGame")
	bool bDoublePointsActivated;

	UPROPERTY(EditDefaultsOnly, Category = "SurvivalGame")
	bool bInstaKillActivated;

	UPROPERTY()
	float TimeBetweenRounds = 5.0f;

	UPROPERTY()
	float TimeBetweenSpawning = 0.5f;

	FTimerHandle TimerHandle_GameTimer;
};
