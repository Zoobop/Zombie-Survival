// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawning/SpawnerManager.h"

// Sets default values
ASpawnerManager::ASpawnerManager()
{
	if (Instance == nullptr) {
		Instance = this;
	}
}

// Called when the game starts or when spawned
void ASpawnerManager::BeginPlay()
{
	Super::BeginPlay();
	
}


