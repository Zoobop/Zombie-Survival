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

void ASoldier::ReceiveStatAttributeModification(TArray<class UStatAttributeModifier*> Modifiers)
{
	/** Send modifiers through Entity Stat Component */
	GetEntityStatComponent()->ApplyStatAttributeModifiers(Modifiers);
}

TArray<class UStatAttributeModifier*> ASoldier::ApplyStatAttributeModification()
{
	TArray<class UStatAttributeModifier*> Modifiers;

	/** Validate current weapon */
	if (EquipmentComponent->GetCurrentWeapon()) {

		AGun* Gun = EquipmentComponent->GetCurrentWeapon();

		/** Add Gun's stat modifier */
		Modifiers.Add(Gun->GetStatAttributeModifier());

		/** Add any of the ammo types' stat modifiers */
		class UStatAttributeModifier* BulletModifier = Gun->GetAmmoType().GetDefaultObject()->GetStatAttributeModifier();

		/** Validate bullet modifier */
		if (BulletModifier)
			Modifiers.Add(BulletModifier);
	}

	return Modifiers;
}

void ASoldier::UseCurrentGun()
{
	AGun* Gun = EquipmentComponent->GetCurrentWeapon();

	if (Gun) {
		if (Gun->Fire()) {
			StartReload();
		}
	}
}

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

void ASoldier::ServerSwitchWeapon_Implementation(int32 Index)
{
	if (bCanSwitchWeapons) {
		EquipmentComponent->ServerPrepWeaponSwap(Index);
		bCanSwitchWeapons = false;
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
		}
	}
}

void ASoldier::ServerOnFire_Implementation()
{
	UseCurrentGun();

	/** Start Timer */
	GetWorldTimerManager().SetTimer(TimeHandle_HandleRefire, this, &ASoldier::UseCurrentGun, EquipmentComponent->GetCurrentWeapon()->GetFireRate(), true);
}

void ASoldier::ServerStopFire_Implementation()
{
	/** Stop Timer */
	GetWorldTimerManager().ClearTimer(TimeHandle_HandleRefire);
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

			/** Start Timer */
			GetWorldTimerManager().SetTimer(TimeHandle_HandleRefire, this, &ASoldier::UseCurrentGun, EquipmentComponent->GetCurrentWeapon()->GetFireRate(), true);
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
		GetWorldTimerManager().ClearTimer(TimeHandle_HandleRefire);
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
