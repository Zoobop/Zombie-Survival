// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PurchasableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
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

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Activate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	FText GetDisplayText() const;

	UFUNCTION()
	virtual bool CheckPlayerPoints(class ASoldier* Player, int32 PointsNeeded) const;

	UFUNCTION()
	virtual void Purchase(class ASoldier* Player) = 0;

protected:

	UFUNCTION()
	virtual void RemovePointsFromPlayer(class ASoldier* Player, int32 PointsToRemove) const;
};
