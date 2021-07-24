// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SurvivalMode.generated.h"


UCLASS()
class SAMPLE_API ASurvivalMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASurvivalMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void PlayerDied(class ASoldier* Killed);

	void UndeadDied(class AUndead* Killed, class ASoldier* Killer);

	void AddSpawnLocation(class AUndeadSpawnPoint* SpawnPoint);

	FORCEINLINE TArray<class AEntityController*> GetAlivePlayers() const { return AlivePlayers; }

protected:

	/** Called at the start of the game */
	virtual void BeginPlay() override;

protected:

	/** Number of undead at a time in the map */
	UPROPERTY(Transient, BlueprintReadOnly)
	int32 MaxSpawnedUndead;

	/** Current players in the game */
	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<class AEntityController*> AlivePlayers;

	/** Current undead in the game */
	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<class AEntityController*> CurrentUndead;

	/** Locations the undead can spawn */
	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<class AUndeadSpawnPoint*> UndeadSpawnLocations;

};
