// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ESSModifierHandlerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UESSModifierHandlerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAMPLE_API IESSModifierHandlerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/** Allows for obtaining stat attribute modifiers through interface */
	UFUNCTION()
	virtual class UStatAttributeModifier* GetStatAttributeModifier() = 0;
};
