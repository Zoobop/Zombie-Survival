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
#include "Components/PointLightComponent.h"



AGun::AGun()
{
	WeaponType = EWeaponType::WPTYPE_GUN;

	MuzzleFlashEffect = CreateDefaultSubobject<UPointLightComponent>("MuzzleFlash");
	MuzzleFlashEffect->SetupAttachment(SkeletalMeshComponent, FireLocationSocketName);
	MuzzleFlashEffect->bEditableWhenInherited = true;
	MuzzleFlashEffect->SetHiddenInGame(true);

	LoadBullets();

	bReplicates = true;
	SetReplicateMovement(true);
}

void AGun::Action(FTransform SpawnTransform, class AEntity* Character)
{

}

bool AGun::LoadBullets()
{
	/** Set ammunition amounts */
	CurrentMagazineAmmo = DefaultAmmoPerMagazine;
	CurrentAmmoReserves = DefaultAmmoPerMagazine * DefaultNumberOfMagazines;

	/** Check ammo */
	CheckFire();

	return !HasFullAmmo();
}

void AGun::ServerLoadBullets_Implementation()
{
	LoadBullets();
}

void AGun::AddModifiers(TArray<class UStatAttributeModifier*> Modifiers)
{
	StatAttributeModifiers.Append(Modifiers);
}

void AGun::ServerAddModifier_Implementation(const TArray<class UStatAttributeModifier*>& Modifiers)
{
	AddModifiers(Modifiers);
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
			//DrawDebugLine(GetWorld(), Player->GetFollowCamera()->GetComponentLocation(), TraceDistance + BulletSpreadAmount, FColor::Magenta, false, .1, 0, 2);

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
						Player->OnEntityHit();
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

	/** Add current list of weapon modifiers */
	Modifiers.Append(StatAttributeModifiers);

	/** Add gun damage to a new modifier and add to list of modifiers */
	Modifiers.Add(GetStatAttributeModifier());

	/** Find and add any bullet modifiers */
	if (ABullet* Bullet = Cast<ABullet>(AmmoType)) {
		StatAttributeModifiers.Add(Bullet->GetStatAttributeModifier());
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
	/** Get modifiers and apply the calculations to the modification value */
	auto Modifiers = ApplyStatAttributeModification();

	/** Check for head shot and apply multiplier */
	if (Hit.BoneName == "head" || Hit.BoneName == "neck_01") {
		UStatAttributeModifier* Modifier = NewObject<UStatAttributeModifier>();
		Modifier->SetupModifier(DefaultDamage * 1.5f, EModificationType::MODTYPE_INSTANT_SINGLE, EOperationType::OPTYPE_SUBTRACT, Tag);
		Modifiers.Add(Modifier);
	}

	/** Get the difference between the distance at which the entity was hit and gun's max falloff range */
	float DistancePastFallOff = Hit.Distance - DefaultDamageFallOffRange;

	if (DistancePastFallOff > 0) {

		/** Calculate the new damage amount after damage fall-off */
		int32 DamageTakeaway = (DistancePastFallOff / DefaultDistanceBetweenIntervals) * DefaultDamageFallOffAmount;

		int32 DamageReductionAfterFallOff = -(DamageTakeaway / Modifiers.Num());
		if (DamageReductionAfterFallOff < -DefaultDamage)
			DamageReductionAfterFallOff = -DefaultDamage - 1;

		/** Apply damage */
		UStatAttributeModifier* Modifier = NewObject<UStatAttributeModifier>();
		Modifier->SetupModifier(DamageReductionAfterFallOff, EModificationType::MODTYPE_INSTANT_SINGLE, EOperationType::OPTYPE_SUBTRACT, Tag);
		Modifiers.Add(Modifier);

	}

	return Modifiers;
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