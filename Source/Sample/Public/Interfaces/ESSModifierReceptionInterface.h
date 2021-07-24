// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ESSModifierReceptionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UESSModifierReceptionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAMPLE_API IESSModifierReceptionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/** Call when receiving damage from another entity */
	UFUNCTION()
	virtual void ReceiveStatAttributeModification(TArray<class UStatAttributeModifier*> Modifiers) = 0;
};
