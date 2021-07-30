// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/WallBuys/MeleeWallBuy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Entities/EquipmentComponent.h"
#include "Entities/Soldier.h"
#include "Items/Weapons/Melee.h"

AMeleeWallBuy::AMeleeWallBuy()
{
	PrepWallBuy();
}

void AMeleeWallBuy::Purchase(class ASoldier* Player)
{
	/** Validate player */
	if (Player) {

		bool bItemFound;

		/** Validate the item */
		if (MeleeToBuy) {

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

bool AMeleeWallBuy::CheckForItem(class ASoldier* Player)
{
	/** Validate player */
	if (Player) {

		UEquipmentComponent* Equipment = Player->GetEquipmentComponent();

		/** Check current melee */
		if (Equipment->GetCurrentMelee()->GetClass() == MeleeToBuy)
			return true;
	}
	return false;
}

void AMeleeWallBuy::PrepWallBuy()
{
	/** Validate gun and item holder */
	if (MeleeToBuy && WallBuyBase) {
		/** Spawn gun on weapon holder */
		AMelee* Melee = Cast<AMelee>(MeleeToBuy);
		ItemName = Melee->GetName();
		WallBuyBase->SetSkeletalMesh(Melee->GetSkeletalMesh());
	}
}

void AMeleeWallBuy::ServerOnFirstTimePurchase_Implementation(class ASoldier* Player)
{

}

void AMeleeWallBuy::ServerOnRefill_Implementation(class ASoldier* Player)
{

}
