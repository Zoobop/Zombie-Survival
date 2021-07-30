// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/PerkStations/PerkStation.h"
#include "RecoveryExpertPerkStation.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API ARecoveryExpertPerkStation : public APerkStation
{
	GENERATED_BODY()
	
public:

	ARecoveryExpertPerkStation();

protected:

	/** Check if player already has perk */
	virtual bool HasPerk(class ASoldier* Player) const override;

	/** Recovery Expert Perk Upgrade */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void RecoveryExpertPerk(class ASoldier* Player);
};
