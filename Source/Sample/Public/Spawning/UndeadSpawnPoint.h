// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UndeadSpawnPoint.generated.h"

UCLASS(BlueprintType)
class SAMPLE_API AUndeadSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUndeadSpawnPoint();

	/** Spawns the undead at a random location within the bounding box */
	UFUNCTION(Server, Reliable)
	void SpawnUndead();

	/** Returns the channel that corresponds with the map debris buy */
	FORCEINLINE int32 GetDebrisChannel() const { return DebrisChannel; }

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Blueprint calculations */
	UFUNCTION(BlueprintImplementableEvent)
	void OnSpawnUndead();

	/** Adds more Health and Speed to the Undead at spawn */
	UFUNCTION(BlueprintPure)
	TArray<class UStatAttributeModifier*> ApplyRoundBasedStats() const;

	/** Register spawn point with the game state */
	void ValidateSpawnPoint();

protected:	

	UPROPERTY(BlueprintReadOnly)
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<class AUndead> UndeadToSpawn;

	/** Debris channel */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Debris")
	int32 DebrisChannel = 0;

};
