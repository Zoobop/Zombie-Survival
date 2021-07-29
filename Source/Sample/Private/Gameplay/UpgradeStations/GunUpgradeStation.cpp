// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/UpgradeStations/GunUpgradeStation.h"
#include "Items/Weapons/Gun.h"
#include "Entities/Soldier.h"
#include "Entities/EquipmentComponent.h"
#include "Entities/EntityState.h"

AGunUpgradeStation::AGunUpgradeStation()
{

}

void AGunUpgradeStation::Purchase(class ASoldier* Player)
{
	Super::Purchase(Player);
}

void AGunUpgradeStation::Upgrade(class ASoldier* Player)
{
	/** Validate current player weapon */
	if (AGun* CurrentWeapon = Player->GetEquipmentComponent()->GetCurrentWeapon()) {

		/** Validate gun */
		if (CurrentWeapon) {

			/** Blueprint event */
			OnUpgrade(Player);

			/** Remove points */
			RemovePointsFromPlayer(Player, UpgradeCost);
		}
	}
}

bool AGunUpgradeStation::HasUpgrade(class ASoldier* Player) const
{
	/** Validate player */
	if (Player) {

		/** Check if current weapon has been upgraded */
		if (AGun* CurrentWeapon = Player->GetEquipmentComponent()->GetCurrentWeapon()) {

			/** Check if weapon has modifiers */
			if (CurrentWeapon->GetWeaponModifiers().Num() == 0)
				return false;

			for (auto UpgradeModifier : UpgradeModifiers) {
				for (auto WeaponModifier : CurrentWeapon->GetWeaponModifiers()) {
					if (UpgradeModifier == WeaponModifier) {
						return true;
					}
				}
			}
		}
	}
	return false;
}
