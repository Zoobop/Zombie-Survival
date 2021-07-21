// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ESSDeathHandlerInterface.generated.h"

/** Blueprints will bind to this event to handle additional functionality on death */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHandleDeath);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UESSDeathHandlerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAMPLE_API IESSDeathHandlerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual void HandleDeath() = 0;

	UFUNCTION()
	virtual bool CheckForDeath(class UStatAttribute* AssociatedStatAttribute) = 0;
};
