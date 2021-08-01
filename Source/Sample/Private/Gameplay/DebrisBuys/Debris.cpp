// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/DebrisBuys/Debris.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Entities/Soldier.h"
#include "Entities/EntityState.h"
#include "Net/UnrealNetwork.h"
#include "Gamemodes/SurvivalGameState.h"


// Sets default values
ADebris::ADebris()
{
	DebrisBase = CreateDefaultSubobject<USceneComponent>("DebrisBase");
	SetRootComponent(DebrisBase);

	DebrisMesh = CreateDefaultSubobject<UStaticMeshComponent>("DebrisMesh");
	DebrisMesh->SetupAttachment(DebrisBase);

	PurchaseTrigger = CreateDefaultSubobject<UBoxComponent>("PurchaseTrigger");
	PurchaseTrigger->SetupAttachment(DebrisMesh);

	DebrisCost = 0;
}


void ADebris::Purchase(class ASoldier* Player)
{
	/** Validate player */
	if (Player) {

		/** Check if player has enough points */
		if (CheckPlayerPoints(Player, DebrisCost)) {
			/** Clear debris */
			RemoveDebris(Player);

			OnDebrisRemoved();
		}
	}
}


void ADebris::ValidateDebris()
{
	/** Get the game state */
	if (ASurvivalGameState* GameState = Cast<ASurvivalGameState>(GetWorld()->GetGameState())) {
		GameState->AddDebrisChannel(this);
	}
}

void ADebris::RemoveDebris(class ASoldier* Player)
{
	/** Moves and hides the debris */
	OnRemoveDebris(Player);

	RemovePointsFromPlayer(Player, DebrisCost);

	ValidateDebris();
}
