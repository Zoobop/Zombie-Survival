// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerManager.generated.h"

/** Singleton */
const static ASpawnerManager* Instance;

UCLASS()
class SAMPLE_API ASpawnerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerManager();

	FORCEINLINE TSubclassOf<class AUndead> GetUndeadToSpawn() const { return UndeadToSpawn; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpawnerManager")
	TArray<class AUndeadSpawnPoint*> SpawnPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpawnerManager")
	TSubclassOf<class AUndead> UndeadToSpawn;
};
