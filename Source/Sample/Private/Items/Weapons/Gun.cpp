// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Gun.h"
#include "Items/Bullets/Bullet.h"
#include "Engine/Classes/Engine/SkeletalMesh.h"
#include "Engine/Classes/Engine/SkeletalMeshSocket.h"
#include "Camera/CameraComponent.h"
#include "Entities/Soldier.h"
#include "Entities/EntityStatComponent.h"
#include "StatAttributes/StatAttributeModifier.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"



AGun::AGun()
{
	LoadBullets();

	bReplicates = true;
	SetReplicateMovement(true);
}

void AGun::Action(FTransform SpawnTransform, class AEntity* Character)
{

}

void AGun::LoadBullets()
{
	/** Set ammunition amounts */
	CurrentMagazineAmmo = DefaultAmmoPerMagazine;
	CurrentAmmoReserves = DefaultAmmoPerMagazine * DefaultNumberOfMagazines;

	/** Check ammo */
	CheckFire();
}

void AGun::SetGunOwner(class AEntity* GunOwner)
{
	Player = GunOwner;
}

bool AGun::Fire()
{
	if (bCanFire && Player) {

		if (HasBulletsToFire()) {
			/** Find gun firing location */
			FVector TraceStart = SkeletalMeshComponent->GetSocketLocation(FireLocationSocketName);

			/** Call blueprint event */
			OnGunFired(Player, TraceStart);

			FVector TraceDistance = Player->GetFollowCamera()->GetComponentLocation() + 
				(Player->GetFollowCamera()->GetComponentRotation().Vector() * DefaultMaxShotDistance);

			/** Bullet spread amount */
			float RandomCoords = FMath::RandRange(-DefaultAccuracy, DefaultAccuracy);
			FVector BulletSpreadAmount = FVector(RandomCoords, RandomCoords, RandomCoords);

			/** Setup ray casting */
			FHitResult HitResult;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);
			QueryParams.AddIgnoredActor(Player);
			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Player->GetFollowCamera()->GetComponentLocation(), TraceDistance + BulletSpreadAmount, ECC_Visibility, QueryParams);

			/** Debug line that shows the bullets trajectory */
			//DrawDebugLine(GetWorld(), TraceStart, TraceDistance + BulletSpreadAmount, FColor::Red, false, .1, 0, 2);
			/** Debug line that shoots straight from the camera */
			DrawDebugLine(GetWorld(), Player->GetFollowCamera()->GetComponentLocation(), TraceDistance + BulletSpreadAmount, FColor::Magenta, false, .1, 0, 2);

			/** Check for hit */
			if (bHit) {
			 
				/** Do whatever happens when the bullet hits... */
				UE_LOG(LogTemp, Warning, TEXT("I hit something %f"), HitResult.GetActor())
			 
				/** Checks for IEntityStatSystemInterface */
				IEntityStatSystemInterface* Interface = Cast<IEntityStatSystemInterface>(HitResult.GetActor());
				if (Interface) {

					/** Apply stat modifiers if not in the same faction */
					if (Player->GetEntityStatComponent()->GetFaction() != Interface->GetEntityStatComponent()->GetFaction()) {
						Interface->ReceiveStatAttributeModification(Player->ApplyStatAttributeModification());

						OnEntityHit(Player, HitResult);
					}
			 
					UE_LOG(LogTemp, Warning, TEXT("Actor has Interface: %f"), Interface)
				}			 
			}

			if (!PostFireCheck()) {
				SetFire(false);
				return true;
			}
		}
	}

	return false;
}

void AGun::SetFire(bool State)
{
	bCanFire = State;
}

bool AGun::PostFireCheck()
{
	/** Decrements current bullet amount */
	CurrentMagazineAmmo--;

	/** Check if there are any more bullets */
	return CurrentMagazineAmmo > 0;
}

class UStatAttributeModifier* AGun::GetStatAttributeModifier()
{
	return StatAttributeModifier;
}

void AGun::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGun, CurrentMagazineAmmo);
	DOREPLIFETIME(AGun, CurrentAmmoReserves);
}

void AGun::CheckFire()
{
	if (HasBulletsToFire() && !HasNoAmmo()) {
		bCanFire = true;
	}
	else {
		bCanFire = false;
	}
}

bool AGun::Reload()
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

	return true;
}