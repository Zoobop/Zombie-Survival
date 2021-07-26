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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_RoundChanged();

	UFUNCTION()
	void StartNextRound();

	UFUNCTION()
	void GameOver();

	FORCEINLINE bool IsDoublePointsActive() const { return bDoublePointsActivated; }

protected:

	void CalculateUndeadPerRound();

	void InitializeRound();

public:

	UPROPERTY(ReplicatedUsing = "OnRep_RoundChanged")
	int32 RoundNumber = 0;

	UPROPERTY()
	int32 UndeadPerRound = 6;

private:

	bool bDoublePointsActivated;

	bool bInstaKillActivated;

	FTimerHandle TimerHandle_RoundCountDown;
};
