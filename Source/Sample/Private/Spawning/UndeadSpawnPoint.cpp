// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawning/UndeadSpawnPoint.h"
#include "Gamemodes/SurvivalGameState.h"
#include "Components/BoxComponent.h"
#include "Entities/Undead.h"

// Sets default values
AUndeadSpawnPoint::AUndeadSpawnPoint()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->bEditableWhenInherited = true;
	BoxCollision->SetupAttachment(RootComponent);
}

void AUndeadSpawnPoint::SpawnUndead_Implementation()
{
	OnSpawnUndead();
}

// Called when the game starts or when spawned
void AUndeadSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	ValidateSpawnPoint();
	
}

TArray<class UStatAttributeModifier*> AUndeadSpawnPoint::ApplyRoundBasedStats() const
{
	/** Get the game state */
	if (ASurvivalGameState* GameState = Cast<ASurvivalGameState>(GetWorld()->GetGameState())) {
		return GameState->GetModifiers();
	}
	return {};
}

void AUndeadSpawnPoint::ValidateSpawnPoint()
{
	/** Get the game state */
	if (ASurvivalGameState* GameState = Cast<ASurvivalGameState>(GetWorld()->GetGameState())) {
		GameState->AddSpawnLocation(this);
	}
}


