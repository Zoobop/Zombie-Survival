// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PurchasableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPurchasableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAMPLE_API IPurchasableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual bool CheckPlayerPoints(class ASoldier* Player, int32 PointsNeeded) const = 0;

	UFUNCTION()
	virtual void Purchase(class ASoldier* Player) = 0;
};
