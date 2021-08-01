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

	FORCEINLINE TArray<class AEntityController*> GetAlivePlayers() const { return AlivePlayers; }

protected:

	/** Called at the start of the game */
	virtual void BeginPlay() override;

protected:

	/** Current players in the game */
	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<class AEntityController*> AlivePlayers;

};
