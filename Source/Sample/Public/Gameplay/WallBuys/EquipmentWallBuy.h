// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/WallBuys/WallBuy.h"
#include "EquipmentWallBuy.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API AEquipmentWallBuy : public AWallBuy
{
	GENERATED_BODY()
	
public:

	AEquipmentWallBuy();

	/** Purchases the display item and gives it to the player */
	virtual void Purchase(class ASoldier* Player) override;

protected:

	/** Checks if the player contains the item */
	virtual bool CheckForItem(class ASoldier* Player) override;

	/** Prepare the wall buy item */
	virtual void PrepWallBuy() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WallBuy")
	TSubclassOf<class AEquipment> EquipmentToBuy;
};
