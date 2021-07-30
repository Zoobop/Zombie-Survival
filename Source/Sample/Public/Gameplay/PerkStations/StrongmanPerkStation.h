// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/PerkStations/PerkStation.h"
#include "StrongManPerkStation.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API AStrongManPerkStation : public APerkStation
{
	GENERATED_BODY()
	
public:

	AStrongManPerkStation();

protected:

	/** Check if player already has perk */
	virtual bool HasPerk(class ASoldier* Player) const override;

	/** Strong Man Perk Upgrade */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StrongManPerk(class ASoldier* Player);

};
