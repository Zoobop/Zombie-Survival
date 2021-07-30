// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PerkStations/PerkStation.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"
#include "Entities/Soldier.h"
#include "Entities/EntityState.h"
#include "Entities/EntityStatComponent.h"

// Sets default values
APerkStation::APerkStation()
{
	StationBase = CreateDefaultSubobject<USceneComponent>("StationBase");
	SetRootComponent(StationBase);

	StationMesh = CreateDefaultSubobject<UStaticMeshComponent>("StationMesh");
	StationMesh->SetupAttachment(StationBase);

	StationEmission = CreateDefaultSubobject<UPointLightComponent>("StationEmission");
	StationEmission->SetupAttachment(StationMesh);
	StationEmission->SetSourceRadius(100.0f);
	StationEmission->SetSoftSourceRadius(200.0f);
	StationEmission->SetIntensity(100.0f);

	PurchaseTrigger = CreateDefaultSubobject<UBoxComponent>("PurchaseTrigger");
	PurchaseTrigger->SetupAttachment(StationMesh);

	PerkCost = 0;
}


void APerkStation::AcquirePerk(class ASoldier* Player)
{
	/** Validate player entity stat component */
	if (UEntityStatComponent* EntityStatComponent = Player->GetEntityStatComponent()) {

		/** Add modifiers to player's entity stat component */
		OnAcquirePerk(Player);

		/** Remove points */
		RemovePointsFromPlayer(Player, PerkCost);
	}
}

void APerkStation::Purchase(class ASoldier* Player)
{
	/** Validate player */
	if (Player) {

		/** Check if player has enough points */
		if (CheckPlayerPoints(Player, PerkCost)) {
			/** Purchase perk */
			AcquirePerk(Player);

			OnPerkAcquisition();
		}
	}
}

bool APerkStation::HasPerk(class ASoldier* Player) const
{
	return false;
}

