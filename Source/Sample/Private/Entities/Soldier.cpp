// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Soldier.h"
#include "Entities/Undead.h"
#include "Entities/EquipmentComponent.h"
#include "Entities/EntityStatComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/Classes/Engine/SkeletalMesh.h"
#include "Engine/Classes/Engine/SkeletalMeshSocket.h"
#include "Items/Weapons/Gun.h"
#include "Items/Bullets/Bullet.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Gamemodes/SurvivalMode.h"

ASoldier::ASoldier()
{
	/** Soldier specific components */
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>("EquipmentComponent");
	EquipmentComponent->bEditableWhenInherited = true;

	/** Weapon Holder and Mesh */
	WeaponHolder = CreateDefaultSubobject<USceneComponent>("WeaponHolder");
	WeaponActor = CreateDefaultSubobject<UChildActorComponent>("WeaponActor");

	WeaponHolder->SetupAttachment(GetMesh(), "EquipSlot");
	WeaponActor->SetupAttachment(WeaponHolder);

	/** Melee Holder and Mesh */
	MeleeHolder = CreateDefaultSubobject<USceneComponent>("MeleeHolder");
	MeleeActor = CreateDefaultSubobject<UChildActorComponent>("MeleeActor");

	MeleeHolder->SetupAttachment(GetMesh(), "MeleeSlot");
	MeleeActor->SetupAttachment(MeleeHolder);

	/** Set game play capabilities */
	bCanSwitchWeapons = true;
	bCanFire = true;
	bCanSprint = true;
	bIsCrouching = false;

	DefaultMovementSpeed = 400.0f;
	CrouchSpeedAmount = 200.0f;
	SprintSpeedAmount = 550.0f;

	GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed;

	/** Set default Entity Stat Component values */
	EntityStatComponent->SoldierDefaults();

}

void ASoldier::UseItem(class AEquipableItem* Item)
{
  	Item->Use(this);
  	Item->OnUse(this);
}

void ASoldier::SendDeathData(AEntity* Killed)
{
	if (HasAuthority()) {
		if (AUndead* Undead = Cast<AUndead>(Killed)) {
			if (ASurvivalMode* GameMode = Cast<ASurvivalMode>(GetWorld()->GetAuthGameMode())) {

				GameMode->UndeadDied(Undead, this);
			}
		}
	}
}

////////////////////////////** --------------- Movement --------------- **//////////////////////////////////

void ASoldier::StartSprint()
{
	if (bCanSprint) {
		bIsSprinting = true;
		bCanFire = false;

		OnStartSprint();

		GetCharacterMovement()->MaxWalkSpeed += SprintSpeedAmount;
	}
}

void ASoldier::StopSprint()
{
	bIsSprinting = false;
	bCanFire = true;

	OnStopSprint();

	GetCharacterMovement()->MaxWalkSpeed -= SprintSpeedAmount;
}

void ASoldier::ToggleCrouch()
{
	/** Check crouch status */
	if (bIsCrouching) {
		StopCrouch();
	}
	else {
		StartCrouch();
	}
}

void ASoldier::StartCrouch_Implementation()
{
	/** Set replicated bool to true */
	bIsCrouching = true;

	/** Call blueprint functionality */
	OnStartCrouching();

	/** Subtract by the crouch amount */
	GetCharacterMovement()->MaxWalkSpeed -= CrouchSpeedAmount;
}

void ASoldier::StopCrouch_Implementation()
{
	/** Set replicated bool to false */
	bIsCrouching = false;

	/** Call blueprint functionality */
	OnStopCrouching();

	/** Add by the crouch amount */
	GetCharacterMovement()->MaxWalkSpeed += CrouchSpeedAmount;
}



void ASoldier::ServerStartReload_Implementation()
{
	AGun* Gun = EquipmentComponent->GetCurrentWeapon();

	/** Validate the current weapon */
	if (Gun) {
		/** Check if ammo is not full and if not already reloading */
		if (!Gun->IsMagazineFull() && !Gun->HasNoAmmo()) {

			/** Set gun firing capability to false */
			Gun->SetFire(false);
			OnGunStartReload(Gun);
		}
	}

}

void ASoldier::StartReload()
{
	/** Check authority */
	if (!HasAuthority()) {
		/** Server call */
		ServerStartReload();
	}
	else {
		AGun* Gun = EquipmentComponent->GetCurrentWeapon();

		/** Validate the current weapon */
		if (Gun) {
			/** Check if ammo is not full and if not already reloading */
			if (!Gun->IsMagazineFull() && !Gun->HasNoAmmo()) {

				/** Set gun firing capability to false */
				Gun->SetFire(false);
				OnGunStartReload(Gun);
			}
		}
	}

}

void ASoldier::FinishReload()
{
	AGun* Gun = EquipmentComponent->GetCurrentWeapon();

	Gun->Reload();
	Gun->SetFire(true);
	
	OnGunReloaded.Broadcast();
}

void ASoldier::CheckReload()
{
	if (AGun* Gun = EquipmentComponent->GetCurrentWeapon()) {		
		if (!Gun->HasBulletsToFire() && !Gun->HasNoAmmo()) {
			StartReload();
		}
	}
}



void ASoldier::ServerADS_Implementation()
{
	/** Set replicated bool to true */
	bIsAiming = true;

	/** Call blueprint implementation */
	OnStartADS();

	/** Adjust movement speed */
	GetCharacterMovement()->MaxWalkSpeed -= 100;
}

void ASoldier::ADS()
{
	if (!HasAuthority()) {
		ServerADS();
	}
	else {
		/** Set replicated bool to true */
		bIsAiming = true;

		/** Call blueprint implementation */
		OnStartADS();

		/** Adjust movement speed */
		GetCharacterMovement()->MaxWalkSpeed -= 100;
	}
}

void ASoldier::ServerStopADS_Implementation()
{
	/** Set replicated bool to false */
	bIsAiming = false;

	/** Call blueprint implementation */
	OnStopADS();

	/** Adjust movement speed */
	GetCharacterMovement()->MaxWalkSpeed += 100;
}

void ASoldier::StopADS()
{
	if (!HasAuthority()) {
		ServerStopADS();
	}
	else {
		/** Set replicated bool to false */
		bIsAiming = false;

		/** Call blueprint implementation */
		OnStopADS();

		/** Adjust movement speed */
		GetCharacterMovement()->MaxWalkSpeed += 100;
	}
}

////////////////////////////** --------------- Weapon Swap --------------- **//////////////////////////////////

void ASoldier::ServerSwitchWeapon_Implementation(int32 Index)
{
	if (bCanSwitchWeapons) {
		EquipmentComponent->PrepWeaponSwap(Index);
		bCanSwitchWeapons = false;
		bIsReloading = false;
		EquipmentComponent->GetCurrentWeapon()->SetFire(false);
	}
}

void ASoldier::NextWeapon()
{
	if (!HasAuthority()) {
		ServerSwitchWeapon(1);
	}
	else {
		if (bCanSwitchWeapons) {
			/** Adds plus 1 to the current weapon index */
			EquipmentComponent->PrepWeaponSwap(1);
			bCanSwitchWeapons = false;
			bIsReloading = false;
			EquipmentComponent->GetCurrentWeapon()->SetFire(false);
		}
	}
}

void ASoldier::PreviousWeapon()
{
	if (!HasAuthority()) {
		ServerSwitchWeapon(-1);
	}
	else {
		if (bCanSwitchWeapons) {
			/** Adds negative 1 to the current weapon index */
			EquipmentComponent->PrepWeaponSwap(-1);
			bCanSwitchWeapons = false;
			bIsReloading = false;
			EquipmentComponent->GetCurrentWeapon()->SetFire(false);
		}
	}
}

void ASoldier::ResetWeaponSwap()
{
	if (AGun* Gun = EquipmentComponent->GetCurrentWeapon()) {
		bCanSwitchWeapons = true;
		bIsReloading = true;
		Gun->SetFire(true);
	}
}



void ASoldier::ServerOnFire_Implementation()
{
	UseCurrentGun();
}

void ASoldier::ServerStopFire_Implementation()
{
	/** Stop Timer */
	EquipmentComponent->GetCurrentWeapon()->StopFiring();
}

void ASoldier::StartFire()
{
	/** Validate the current weapon */
	if (EquipmentComponent->GetCurrentWeapon()) {

		if (!HasAuthority()) {
			/** Fire weapon */
			ServerOnFire();
		}
		else {
			/** Fire weapon */
			UseCurrentGun();
		}
	}
}

void ASoldier::StopFire()
{
	if (!HasAuthority()) {
		ServerStopFire();
	}
	else {
		/** Stop Timer */
		EquipmentComponent->GetCurrentWeapon()->StopFiring();
	}
}

void ASoldier::UseCurrentGun()
{
	if (AGun* Gun = EquipmentComponent->GetCurrentWeapon()) {
		Gun->ShootGun();
	}
}



void ASoldier::BeginPlay()
{
	Super::BeginPlay();
}

void ASoldier::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ASoldier::StartFire);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &ASoldier::StopFire);

	PlayerInputComponent->BindAction("ADS", IE_Pressed, this, &ASoldier::ADS);
	PlayerInputComponent->BindAction("ADS", IE_Released, this, &ASoldier::StopADS);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ASoldier::StartReload);

	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &ASoldier::NextWeapon);
	PlayerInputComponent->BindAction("PreviousWeapon", IE_Pressed, this, &ASoldier::PreviousWeapon);

	//PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASoldier::StartSprint);
	//PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASoldier::StopSprint);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, & ASoldier::ToggleCrouch);
}

void ASoldier::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASoldier, bIsAiming);
	DOREPLIFETIME(ASoldier, bIsReloading);
	DOREPLIFETIME(ASoldier, bIsCrouching);
	DOREPLIFETIME(ASoldier, bCanSwitchWeapons);
}
