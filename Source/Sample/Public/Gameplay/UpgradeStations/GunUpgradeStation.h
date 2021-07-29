// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/UpgradeStations/UpgradeStation.h"
#include "GunUpgradeStation.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API AGunUpgradeStation : public AUpgradeStation
{
	GENERATED_BODY()
	
public:

	AGunUpgradeStation();

	/** Purchases the display item and gives it to the player */
	UFUNCTION(BlueprintCallable)
	void Purchase(class ASoldier* Player) override;

	/** Upgrade the current weapon */
	virtual void Upgrade(class ASoldier* Player) override;

protected:

	/** Checks if the item has been upgrade already */
	virtual bool HasUpgrade(class ASoldier* Player) const override;
};
