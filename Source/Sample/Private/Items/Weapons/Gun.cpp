// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Gun.h"
#include "Items/Bullets/Bullet.h"
#include "Engine/Classes/Engine/SkeletalMesh.h"
#include "Engine/Classes/Engine/SkeletalMeshSocket.h"
#include "Entities/Entity.h"
#include "DrawDebugHelpers.h"

UGun::UGun()
{
	PreLoad();
};

void UGun::Action(FTransform SpawnTransform, class AEntity* Character)
{

}

void UGun::PreLoad()
{
	/** Set ammunition amounts */
	CurrentMagazineAmmo = DefaultAmmoPerMagazine;
	CurrentAmmoReserves = DefaultAmmoPerMagazine * DefaultNumberOfMagazines;
}

bool UGun::PostFireCheck()
{
	/** Decrements current bullet amount */
	CurrentMagazineAmmo--;

	/** Call the event for the after effects */
	OnGunFired.Broadcast();

	/** Check if there are any more bullets */
	return CurrentMagazineAmmo > 0;
}

class UStatAttributeModifier* UGun::GetStatAttributeModifier()
{
	return StatAttributeModifier;
}

bool UGun::Reload()
{
	/** Checks if there is more ammo in reserve */
	if ((CurrentAmmoReserves == 0 && CurrentMagazineAmmo == 0) || CurrentMagazineAmmo == DefaultAmmoPerMagazine)
		return false;

	/** Ammo count difference */
	int32 AmmoDifference = DefaultAmmoPerMagazine - CurrentMagazineAmmo;

	/** Check if there is enough ammo in reserves */
	if (CurrentAmmoReserves < AmmoDifference) {
		/** Add the remaining ammo reserves to the current magazine ammo amount */
		CurrentMagazineAmmo += CurrentAmmoReserves;
		CurrentAmmoReserves = 0;
	}
	else {
		/** Modify ammo counts by the difference */
		CurrentMagazineAmmo += AmmoDifference;
		CurrentAmmoReserves -= AmmoDifference;
	}

	/** Call the event for the after effects */
	OnGunFired.Broadcast();

	return true;
}