// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SurvivalMode.generated.h"


/** Singleton */
const static ASurvivalMode* Instance;

UCLASS()
class SAMPLE_API ASurvivalMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASurvivalMode();

protected:

	/** Called at the start of the game */
	virtual void BeginPlay() override;

protected:

	/** Round number */
	int32 RoundNumber;

	/** Number of undead at a time in the map */
	int32 MaxSpawnedUndead;

	/** Current players in the game */
	TArray<class ASoldier*> Survivors;

	/** Current undead in the game */
	TArray<class AUndead*> CurrentUndead;

	/** Locations the undead can spawn */
	TArray<int32> UndeadSpawnLocations;
};
