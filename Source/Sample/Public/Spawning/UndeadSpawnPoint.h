// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UndeadSpawnPoint.generated.h"

UCLASS()
class SAMPLE_API AUndeadSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUndeadSpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ValidateSpawnPoint();

protected:	

	UPROPERTY()
	class UBoxComponent* BoxCollision;

};
