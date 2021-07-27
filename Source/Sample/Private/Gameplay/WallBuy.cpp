// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/WallBuy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Entities/Soldier.h"
#include "Items/Armor/Armor.h"
#include "Items/Equipment/Lethal.h"
#include "Items/Equipment/Tactical.h"
#include "Items/Equipment/Consumable.h"
#include "Entities/EquipmentComponent.h"
#include "Entities/EntityState.h"

// Sets default values
AWallBuy::AWallBuy()
{
	WallBuyBase = CreateDefaultSubobject<USkeletalMeshComponent>("ItemMesh");
	SetRootComponent(WallBuyBase);

	PurchaseTrigger = CreateDefaultSubobject<UBoxComponent>("PurchaseTrigger");
	PurchaseTrigger->SetupAttachment(WallBuyBase);

	WallBuyHighlight = CreateDefaultSubobject<UPointLightComponent>("WallBuyHighlight");
	WallBuyHighlight->SetupAttachment(WallBuyBase);
	WallBuyHighlight->SetSourceRadius(100.0f);
	WallBuyHighlight->SetSoftSourceRadius(200.0f);
	WallBuyHighlight->SetIntensity(100.0f);
}

void AWallBuy::Purchase(class ASoldier* Player)
{

}


bool AWallBuy::CheckPlayerPoints(class ASoldier* Player, int32 PointsNeeded) const
{
	/** Validate player */
	if (Player) {

		/** Validate and check points */
		if (AEntityState* State = Cast<AEntityState>(Player->GetPlayerState())) {
			if (State->GetCurrentPoints() >= PointsNeeded)
				return true;
		}
	}
	return false;
}


// Called when the game starts or when spawned
void AWallBuy::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AWallBuy::CheckForItem(class ASoldier* Player)
{
	/** Validate player */
	if (Player) {

		UEquipmentComponent* Equipment = Player->GetEquipmentComponent();

		/** Check equipped armor */
		if (Equipment->GetEquippedArmor().Num() > 0){
			for (auto ArmorPair : Equipment->GetEquippedArmor()) {
				if (ArmorPair.Value->GetClass() == ItemToBuy)
					return true;
			}
		}

		/** Check equipped lethals */
		if (Equipment->GetEquippedLethals().Num() > 0) {
			for (auto Lethal : Equipment->GetEquippedLethals()) {
				if (Lethal->GetClass() == ItemToBuy)
					return true;
			}
		}

		/** Check equipped tacticals */
		if (Equipment->GetEquippedTacticals().Num() > 0) {
			for (auto Tactical : Equipment->GetEquippedTacticals()) {
				if (Tactical->GetClass() == ItemToBuy)
					return true;
			}
		}

	  	/** Check equipped consumables */
 		if (Equipment->GetEquippedConsumables().Num() > 0) {
  			for (auto Consumable : Equipment->GetEquippedConsumables()) {
  				if (Consumable->GetClass() == ItemToBuy)
  					return true;
  			}
 		}

	}
	return false;
}
