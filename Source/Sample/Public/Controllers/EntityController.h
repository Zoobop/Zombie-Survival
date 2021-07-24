// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EntityController.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API AEntityController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(BlueprintReadOnly)
	FName PlayerName = "Player";

};
