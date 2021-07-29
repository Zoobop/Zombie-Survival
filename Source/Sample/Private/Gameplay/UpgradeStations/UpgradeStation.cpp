// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/UpgradeStations/UpgradeStation.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"
#include "Entities/Soldier.h"
#include "Entities/EntityState.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AUpgradeStation::AUpgradeStation()
{
	StationMesh = CreateDefaultSubobject<UStaticMeshComponent>("StationMesh");
	SetRootComponent(StationMesh);

	StationEmission = CreateDefaultSubobject<UPointLightComponent>("StationEmission");
	StationEmission->SetupAttachment(StationMesh);
	StationEmission->SetSourceRadius(100.0f);
	StationEmission->SetSoftSourceRadius(200.0f);
	StationEmission->SetIntensity(100.0f);

	PurchaseTrigger = CreateDefaultSubobject<UBoxComponent>("PurchaseTrigger");
	PurchaseTrigger->SetupAttachment(StationMesh);

	UpgradeCost = 5000;
}

void AUpgradeStation::Purchase(class ASoldier* Player)
{
	/** Validate player */
	if (Player) {

		/** Check if player has enough points */
		if (CheckPlayerPoints(Player, UpgradeCost)) {
			/** Purchase upgrade */
			Upgrade(Player);

			OnPostPurchase();
		}
	}
}

// Called when the game starts or when spawned
void AUpgradeStation::BeginPlay()
{
	Super::BeginPlay();
}

void AUpgradeStation::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUpgradeStation, UpgradeModifiers);
}

bool AUpgradeStation::HasUpgrade(class ASoldier* Player) const
{
	return false;
}

