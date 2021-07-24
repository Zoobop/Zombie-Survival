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
#include "Entities/EntityState.h"



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
	Player = Cast<ASoldier>(GunOwner);
}

void AGun::ShootGun()
{
	if (!bIsAutomatic) {
		Fire();
	}
	else {
		Fire();

		/** Start Timer */
		GetWorldTimerManager().SetTimer(TimeHandle_HandleRefire, this, &AGun::Fire, GetFireRate(), true);
	}
}

void AGun::Fire()
{
	if (bCanFire && Player) {

		if (HasBulletsToFire()) {
			/** Find gun firing location */
			FVector TraceStart = SkeletalMeshComponent->GetSocketLocation(FireLocationSocketName);

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
				UE_LOG(LogTemp, Warning, TEXT("Entity Hit: %f"), HitResult.GetActor())
			 
				/** Checks for IEntityStatSystemInterface and IESSModifierReceptionInterface */
				IEntityStatSystemInterface* Interface = Cast<IEntityStatSystemInterface>(HitResult.GetActor());
				if (Interface) {
					/** Apply stat modifiers if not in the same faction */
					if (Player->GetEntityStatComponent()->GetFaction() != Interface->GetEntityStatComponent()->GetFaction()) {
						Interface->GetEntityStatComponent()->SetDamageDealer(Player);

						/** Calculates damage base on distance and hit location */
						TArray<UStatAttributeModifier*> Modifiers = CalculateHit(HitResult);

						/** Apply the modifiers to the entity */
						Interface->GetEntityStatComponent()->ReceiveStatAttributeModification(Modifiers);

						/** Get total damage */
						int32 TotalDamage = 0;
						for (auto Modifier : Modifiers) {
							TotalDamage += Modifier->GetModificationAmount();
						}
						OnEntityHit(Player, TotalDamage, HitResult);
					}
			 
					UE_LOG(LogTemp, Warning, TEXT("Actor has Interface: %f"), Interface)
				}			 
			}

			/** Invoke event */
			OnGunStartFire(Player);
			OnGunFired.Broadcast();

			if (!PostFireCheck()) {
				Player->CheckReload();
			}
		}
	}
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

void AGun::StopFiring()
{
	if (TimeHandle_HandleRefire.IsValid())
		GetWorldTimerManager().ClearTimer(TimeHandle_HandleRefire);
}

TArray<class UStatAttributeModifier*> AGun::ApplyStatAttributeModification()
{
	TArray<class UStatAttributeModifier*> Modifiers;

	/** Add gun damage to a new modifier and add to list */
	Modifiers.Add(GetStatAttributeModifier());

	/** Find and add any bullet modifiers */
	if (ABullet* Bullet = Cast<ABullet>(AmmoType)) {
		Modifiers.Add(Bullet->GetStatAttributeModifier());
	}

	return Modifiers;
}

class UStatAttributeModifier* AGun::GetStatAttributeModifier()
{
	UStatAttributeModifier* Modifier = NewObject<UStatAttributeModifier>();
	Modifier->SetupModifier(DefaultDamage, EModificationType::MODTYPE_INSTANT_SINGLE, EOperationType::OPTYPE_SUBTRACT, Tag);
	return Modifier;
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

TArray<class UStatAttributeModifier*> AGun::CalculateHit(FHitResult Hit)
{
	/** Get the difference between the distance at which the entity was hit and gun's max falloff range */
	float DistancePastFallOff = Hit.Distance - DefaultDamageFallOffRange;

	if (DistancePastFallOff > 0) {

		/** Calculate the new damage amount after damage fall-off */
		int32 DamageTakeaway = (DistancePastFallOff / DefaultDistanceBetweenIntervals) * DefaultDamageFallOffAmount;

		/** Get modifiers and apply the calculations to the modification value */
		auto Modifiers = ApplyStatAttributeModification();

		int32 NewDamage = -(DamageTakeaway / Modifiers.Num());
		if (NewDamage < -DefaultDamage)
			NewDamage = -DefaultDamage - 1;

		/** Apply damage */
		UStatAttributeModifier* Modifier = NewObject<UStatAttributeModifier>();
		Modifier->SetupModifier(NewDamage, EModificationType::MODTYPE_INSTANT_SINGLE, EOperationType::OPTYPE_SUBTRACT, Tag);
		Modifiers.Add(Modifier);

		return Modifiers;
	}

	return ApplyStatAttributeModification();
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