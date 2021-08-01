
// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Undead.h"
#include "Entities/EntityStatComponent.h"
#include "Gamemodes/SurvivalGameState.h"

AUndead::AUndead()
{
	GetEntityStatComponent()->UndeadDefaults();
}

TArray<class UStatAttributeModifier*> AUndead::ApplyStatAttributeModification()
{
	TArray<class UStatAttributeModifier*> Modifiers;

	Modifiers.Add(StatAttributeModifier);

	return Modifiers;
}

void AUndead::BeginPlay()
{
	Super::BeginPlay();

	ValidateUndead();
}

void AUndead::ValidateUndead()
{
	/** Get the game state */
	if (ASurvivalGameState* GameState = Cast<ASurvivalGameState>(GetWorld()->GetGameState())) {
		GameState->AddSpawnedUndead(this);
	}
}

