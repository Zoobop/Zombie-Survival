// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawning/UndeadSpawnPoint.h"
#include "Gamemodes/SurvivalMode.h"
#include "Components/BoxComponent.h"

// Sets default values
AUndeadSpawnPoint::AUndeadSpawnPoint()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->bEditableWhenInherited = true;
	BoxCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AUndeadSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	ValidateSpawnPoint();
	
}

void AUndeadSpawnPoint::ValidateSpawnPoint()
{
	Cast<ASurvivalMode>(GetWorld()->GetAuthGameMode())->AddSpawnLocation(this);
}

