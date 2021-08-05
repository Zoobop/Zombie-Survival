// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/WallBuys/EquipmentWallBuy.h"
#include "Entities/EquipmentComponent.h"
#include "Entities/Soldier.h"
#include "Items/Equipment/Lethal.h"

AEquipmentWallBuy::AEquipmentWallBuy()
{
	PrepWallBuy();
}

void AEquipmentWallBuy::Purchase(class ASoldier* Player)
{
	/** Validate player */
	if (Player) {

		bool bItemFound;

		/** Validate the item */
		if (EquipmentToBuy) {

			/** Find if player already contains the item */
			bItemFound = CheckForItem(Player);

			if (bItemFound) {
				/** Check points */
				if (CheckPlayerPoints(Player, SecondaryPrice)) {
					/** Purchase at refill price */
					OnRefill(Player);
				}
			}
			else {
				/** Check points */
				if (CheckPlayerPoints(Player, PrimaryPrice)) {
					/** Purchase at full price */
					OnFirstTimePurchase(Player);
				}
			}
		}
	}
}

bool AEquipmentWallBuy::CheckForItem(class ASoldier* Player)
{
	/** Validate player */
	if (Player) {

		UEquipmentComponent* Equipment = Player->GetEquipmentComponent();

		/** Check equipped weapons */
		if (Equipment->GetEquippedLethal()->GetClass() == EquipmentToBuy) {
			return true;
		}
	}
	return false;
}

void AEquipmentWallBuy::PrepWallBuy()
{
	/** Validate gun and item holder */
	if (EquipmentToBuy && WallBuyBase) {
		/** Spawn gun on weapon holder */
		AEquipment* Equipment = Cast<AEquipment>(EquipmentToBuy);
		ItemName = Equipment->GetName();
		WallBuyBase->SetSkeletalMesh(Equipment->GetSkeletalMesh());
	}
}
