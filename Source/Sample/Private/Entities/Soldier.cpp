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
#include "Gamemodes/SurvivalGameState.h"

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
	bCanSprint = true;
	bCanMelee = true;
	bCanReload = true;
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
			if (ASurvivalGameState* GameState = Cast<ASurvivalGameState>(GetWorld()->GetGameState())) {
				GameState->UndeadKilled(Undead, this);
			}
		}
	}
}

////////////////////////////** --------------- Movement --------------- **//////////////////////////////////

void ASoldier::StartSprint()
{
	if (bCanSprint) {
		bIsSprinting = true;

		OnStartSprint();

		GetCharacterMovement()->MaxWalkSpeed += SprintSpeedAmount;
	}
}

void ASoldier::StopSprint()
{
	bIsSprinting = false;

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
	/** Validate the current weapon */
	if (AGun* Gun = EquipmentComponent->GetCurrentWeapon()) {
		/** Check if ammo is not full and if not already reloading */
		if (!Gun->IsMagazineFull() && !Gun->HasNoAmmo() && bCanReload) {
			bCanReload = false;
			bCanSwitchWeapons = false;

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
		/** Validate the current weapon */
		if (AGun* Gun = EquipmentComponent->GetCurrentWeapon()) {
			/** Check if ammo is not full and if not already reloading */
			if (!Gun->IsMagazineFull() && !Gun->HasNoAmmo() && bCanReload) {
				bCanReload = false;
				bCanSwitchWeapons = false;

				/** Set gun firing capability to false */
				Gun->SetFire(false);
				OnGunStartReload(Gun);
			}
		}
	}

}

void ASoldier::FinishReload()
{
	if (AGun* Gun = EquipmentComponent->GetCurrentWeapon()) {
		Gun->Reload();
		Gun->SetFire(true);
		bCanReload = true;
		bCanSwitchWeapons = true;
	
		OnGunReloaded.Broadcast();
	}
}

void ASoldier::ResetReload(bool State)
{
	bCanReload = State;
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



void ASoldier::ServerInteract_Implementation()
{
	StartInteract();
}

void ASoldier::Interact()
{
	/** Check authority */
	if (!HasAuthority()) {
		ServerInteract();
	}
	else {
		StartInteract();
	}
}

////////////////////////////** --------------- Weapon Swap --------------- **//////////////////////////////////

void ASoldier::ServerSwitchWeapon_Implementation(int32 Index)
{
	if (EquipmentComponent->HasMultipleWeapons() && bCanSwitchWeapons) {
		EquipmentComponent->PrepWeaponSwap(Index);
		bCanSwitchWeapons = false;
		bCanReload = false;
		bCanMelee = false;
		EquipmentComponent->GetCurrentWeapon()->SetFire(false);
	}
}

void ASoldier::NextWeapon()
{
	if (!HasAuthority()) {
		ServerSwitchWeapon(1);
	}
	else {
		if (EquipmentComponent->HasMultipleWeapons() && bCanSwitchWeapons) {
			/** Adds plus 1 to the current weapon index */
			EquipmentComponent->PrepWeaponSwap(1);
			bCanSwitchWeapons = false;
			bCanReload = false;
			bCanMelee = false;
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
		if (EquipmentComponent->HasMultipleWeapons() && bCanSwitchWeapons) {
			/** Adds negative 1 to the current weapon index */
			EquipmentComponent->PrepWeaponSwap(-1);
			bCanSwitchWeapons = false;
			bCanReload = false;
			bCanMelee = false;
			EquipmentComponent->GetCurrentWeapon()->SetFire(false);
		}
	}
}

void ASoldier::ResetWeaponSwap()
{
	if (AGun* Gun = EquipmentComponent->GetCurrentWeapon()) {
		bCanSwitchWeapons = true;
		bCanReload = true;
		bCanMelee = true;
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



void ASoldier::ServerOnMelee_Implementation()
{
	if (bCanMelee) {
		if (AGun* Gun = EquipmentComponent->GetCurrentWeapon()) {
			bCanMelee = false;
			bCanSwitchWeapons = false;
			bCanReload = false;
			Gun->SetFire(false);
			EquipmentComponent->SetMeleeVisibility(false);
			OnStartMelee();
		}
	}
}

void ASoldier::ServerStopMelee_Implementation()
{
	if (AGun* Gun = EquipmentComponent->GetCurrentWeapon()) {
		bCanMelee = true;
		bCanSwitchWeapons = true;
		bCanReload = true;
		EquipmentComponent->SetMeleeVisibility(true);
		Gun->SetFire(true);
	}
}

void ASoldier::StartMelee()
{
	if (!HasAuthority()) {
		ServerOnMelee();
	}
	else {
		if (bCanMelee) {
			if (AGun* Gun = EquipmentComponent->GetCurrentWeapon()) {
				bCanMelee = false;
				bCanSwitchWeapons = false;
				bCanReload = false;
				EquipmentComponent->SetMeleeVisibility(false);
				Gun->SetFire(false);
				OnStartMelee();
			}
		}
	}
}

void ASoldier::FinishMelee()
{
	if (!HasAuthority()) {
		ServerStopMelee();
	}
	else {
		if (AGun* Gun = EquipmentComponent->GetCurrentWeapon()) {
			bCanMelee = true;
			bCanSwitchWeapons = true;
			bCanReload = true;
			EquipmentComponent->SetMeleeVisibility(true);
			Gun->SetFire(true);
		}
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

	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &ASoldier::StartMelee);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASoldier::Interact);

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
	DOREPLIFETIME(ASoldier, bCanMelee);
	DOREPLIFETIME(ASoldier, bCanReload);
	DOREPLIFETIME(ASoldier, bCanInteract);
}
