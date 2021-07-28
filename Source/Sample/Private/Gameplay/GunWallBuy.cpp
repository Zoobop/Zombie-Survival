// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GunWallBuy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Entities/EquipmentComponent.h"
#include "Entities/Soldier.h"
#include "Items/Weapons/Gun.h"

AGunWallBuy::AGunWallBuy()
{
	PrepWallBuy();
}

void AGunWallBuy::Purchase(class ASoldier* Player)
{
	/** Validate player */
	if (Player) {

		bool bItemFound;

		/** Validate the item */
		if (GunToBuy) {

			/** Find if player already contains the item */
			bItemFound = CheckForItem(Player);

			if (bItemFound) {
				/** Check points */
				if (CheckPlayerPoints(Player, SecondaryPrice)) {
					/** Purchase at refill price */
					OnRefill(Player);
					ServerOnRefill(Player);
				}
			}
			else {
				/** Check points */
				if (CheckPlayerPoints(Player, PrimaryPrice)) {
					/** Purchase at full price */
					OnFirstTimePurchase(Player);
					ServerOnFirstTimePurchase(Player);
				}
			}
		}
	}
}

bool AGunWallBuy::CheckForItem(class ASoldier* Player)
{
	/** Validate player */
	if (Player) {

		UEquipmentComponent* Equipment = Player->GetEquipmentComponent();

		/** Check equipped weapons */
		if (Equipment->GetEquippedWeapons().Num() > 0) {
			for (AGun* Gun : Equipment->GetEquippedWeapons()) {
				if (Gun->GetClass() == GunToBuy)
					return true;
			}
		}
	}
	return false;
}

void AGunWallBuy::PrepWallBuy()
{
	/** Validate gun and item holder */
	if (GunToBuy && WallBuyBase) {
		/** Spawn gun on weapon holder */
		AGun* Gun = Cast<AGun>(GunToBuy);
		ItemName = Gun->GetName();
		WallBuyBase->SetSkeletalMesh(Gun->GetSkeletalMesh());
	}
}

void AGunWallBuy::ServerOnFirstTimePurchase_Implementation(class ASoldier* Player)
{
	/** Validate player */
	if (Player) {

		/** Adds the weapon to equipped weapons */
		Player->GetEquipmentComponent()->AddGun(GunToBuy);
		
	}
}

void AGunWallBuy::ServerOnRefill_Implementation(class ASoldier* Player)
{
	/** Validate player */
	if (Player) {

	}
}
