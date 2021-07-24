// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "EntityState.generated.h"

UCLASS()
class SAMPLE_API AEntityState : public APlayerState
{
	GENERATED_BODY()
	
public:

	AEntityState();

	/** Call this function to give a player points */
	UFUNCTION(BlueprintCallable)
	void AddPoints(int32 Points);

	/** Call this function remove points from the player */
	UFUNCTION(BlueprintCallable)
	void RemovePoints(int32 Points);

	/** Call this function to add to players' kill counter */
	UFUNCTION(BlueprintCallable)
	void IncrementKillTotal();

	FORCEINLINE int32 GetCurrentPoints() const { return CurrentPoints; }

public:
	
	const static int32 PointsPerKill = 60;
	const static int32 HeadshotBonus = 40;
	const static int32 PointsPerHit = 10;

protected:

	UPROPERTY(BlueprintReadOnly)
	int32 Kills = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 TotalPoints = 500;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPoints = 500;

};
