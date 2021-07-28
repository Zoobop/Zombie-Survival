// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/WallBuy.h"
#include "GunWallBuy.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API AGunWallBuy : public AWallBuy
{
	GENERATED_BODY()
	
public:

	AGunWallBuy();

	/** Purchases the display item and gives it to the player */
	virtual void Purchase(class ASoldier* Player) override;

protected:

	/** Checks if the player contains the item */
	virtual bool CheckForItem(class ASoldier* Player) override;

	/** Prepare the wall buy item */
	virtual void PrepWallBuy() override;

	/** Server First Time Purchase */
	UFUNCTION(Server, Reliable)
	void ServerOnFirstTimePurchase(class ASoldier* Player);

	/** Server Refill */
	UFUNCTION(Server, Reliable)
	void ServerOnRefill(class ASoldier* Player);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WallBuy")
	TSubclassOf<class AGun> GunToBuy;
};
