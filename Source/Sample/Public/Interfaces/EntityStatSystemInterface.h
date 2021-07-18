// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EntityStatSystemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UEntityStatSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAMPLE_API IEntityStatSystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/** Allows for safe referencing without Actor casting */
	UFUNCTION()
	virtual class UEntityStatComponent* GetEntityStatComponent() = 0;

	/** Call when receiving damage from another entity */
	UFUNCTION()
	virtual void ReceiveStatAttributeModification(TArray<class UStatAttributeModifier*> Modifiers) = 0;

	/** Call when applying damage to another entity */
	UFUNCTION()
	virtual TArray<class UStatAttributeModifier*> ApplyStatAttributeModification() = 0;
};
