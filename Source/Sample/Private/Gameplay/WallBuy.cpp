// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/WallBuy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Entities/Soldier.h"
#include "Items/Weapons/Gun.h"
#include "Items/Armor/Armor.h"
#include "Items/Weapons/Gun.h"
#include "Items/Weapons/Melee.h"
#include "Items/Equipment/Lethal.h"
#include "Items/Equipment/Tactical.h"
#include "Items/Equipment/Consumable.h"
#include "Entities/EquipmentComponent.h"
#include "Entities/EntityState.h"

// Sets default values
AWallBuy::AWallBuy()
{
	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ItemMesh");
	SetRootComponent(ItemMesh);

	PurchaseTrigger = CreateDefaultSubobject<UBoxComponent>("PurchaseTrigger");
	PurchaseTrigger->SetupAttachment(ItemMesh);
}

void AWallBuy::Purchase(class ASoldier* Player)
{
	/** Validate player */
	if (Player) {

		bool bItemFound;

		/** Validate the item */
		if (ItemToBuy) {

			/** Find if player already contains the item */
			bItemFound = CheckItem(Player);
			
			if (bItemFound) {
				/** Check points */
				if (CheckPlayerPoints(Player, RefillPrice)) {
					/** Purchase at refill price */
					OnRefill(Player);
				}
			}
			else {
				/** Check points */
				if (CheckPlayerPoints(Player, PurchasePrice)) {
					/** Purchase at full price */
					OnFirstTimePurchase(Player);
				}
			}
		}
	}
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

bool AWallBuy::CheckItem(class ASoldier* Player)
{
	/** Validate player */
	if (Player) {

		UEquipmentComponent* Equipment = Player->GetEquipmentComponent();

		/** Check current melee */
		if (Equipment->GetCurrentMelee()->GetClass() == ItemToBuy)
			return true;

		/** Check equipped weapons */
		if (Equipment->GetEquippedWeapons().Num() > 0) {
			for (auto Gun : Equipment->GetEquippedWeapons()) {
				if (Gun->GetClass() == ItemToBuy)
					return true;
			}
		}

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
