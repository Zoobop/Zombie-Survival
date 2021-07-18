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
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");

	WeaponHolder->SetupAttachment(GetMesh(), "EquipSlot");
	WeaponMesh->SetupAttachment(WeaponHolder);

	/** Set game play capabilities */
	bCanFire = true;
	bCanSprint = true;
	bCanReload = false;
	bIsCrouching = false;

	DefaultMovementSpeed = 400.0f;
	CrouchSpeedAmount = 200.0f;
	SprintSpeedAmount = 550.0f;

	GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed;

	/** Set default Entity Stat Component values */
	EntityStatComponent->SoldierDefaults();

}

void ASoldier::UseItem(class UEquipableItem* Item)
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

		UGun* Gun = EquipmentComponent->GetCurrentWeapon();

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
	if (bCanFire) {
		/** Find weapon fire location */
		FTransform SpawnTransform = WeaponHolder->GetComponentTransform();
		SpawnTransform.SetLocation(FollowCamera->GetComponentRotation().Vector() * 10 + WeaponHolder->GetSocketLocation("Muzzle"));

		/** Local variable of the current weapon for ease of writing */
		const auto Gun = EquipmentComponent->GetCurrentWeapon();

		/** Check if there are bullets to shoot */
		if (Gun->HasBulletsToFire()) {

			/** Find gun muzzle location location */
			FVector TraceEnd = FollowCamera->GetComponentLocation() + (FollowCamera->GetComponentRotation().Vector() * 10000.0f);

			/** Setup ray casting */
			FHitResult HitResult;
			FCollisionQueryParams QueryParams;
			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, FollowCamera->GetComponentLocation(), TraceEnd, ECC_Visibility, QueryParams);

			/** Check for hit */
			if (bHit) {

				/** Do whatever happens when the bullet hits... */
				UE_LOG(LogTemp, Warning, TEXT("I hit something %f"), HitResult.GetActor())

				CheckRayCastActor(HitResult.GetActor());

				/** Checks for IEntityStatSystemInterface */
				IEntityStatSystemInterface* Interface = Cast<IEntityStatSystemInterface>(HitResult.GetActor());
				if (Interface) {

					/** Apply stat modifiers if not in the same faction */
					if (GetEntityStatComponent()->GetFaction() != Interface->GetEntityStatComponent()->GetFaction()) {
						Interface->ReceiveStatAttributeModification(ApplyStatAttributeModification());

					}

					UE_LOG(LogTemp, Warning, TEXT("Actor has Interface: %f"), Interface)

				}

				/** Invoke gun event */
				OnGunFired.Broadcast(HitResult);
			}

			/** Debug line to visualize the bullet */
			DrawDebugLine(GetWorld(), FollowCamera->GetComponentLocation(), TraceEnd, FColor::Red, false, 1.0f); /// From Camera
			DrawDebugLine(GetWorld(), SpawnTransform.GetLocation(), TraceEnd, FColor::Emerald, false, 1.0f); /// From Gun Muzzle

			/** Check current ammo count -- if PostFireCheck fails, the gun is force to reload */
			if (!Gun->PostFireCheck()) {
				OnGunStartReload.Broadcast(Gun);
			}
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

void ASoldier::ReloadCurrentGun()
{
	/** Validate the current weapon */
	if (EquipmentComponent->GetCurrentWeapon()) {

		/** Set reload bool to true */
		bIsReloading = true;

		/** Try to reload weapon */
		OnGunStartReload.Broadcast(EquipmentComponent->GetCurrentWeapon());
	}
}

void ASoldier::ADS_Implementation()
{
	/** Set replicated bool to true */
	bIsAiming = true;

	/** Call blueprint implementation */
	OnStartADS();

	/** Adjust movement speed */
	GetCharacterMovement()->MaxWalkSpeed -= 100;
}


void ASoldier::StopADS_Implementation()
{
	/** Set replicated bool to false */
	bIsAiming = false;

	/** Call blueprint implementation */
	OnStopADS();

	/** Adjust movement speed */
	GetCharacterMovement()->MaxWalkSpeed += 100;
}

void ASoldier::NextWeapon()
{
	/** Adds plus 1 to the current weapon index */
	EquipmentComponent->SwitchWeapon(1);
}

void ASoldier::PreviousWeapon()
{
	/** Adds negative 1 to the current weapon index */
	EquipmentComponent->SwitchWeapon(-1);
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

// FVector ASoldier::GetPawnViewLocation() const
// {
// 	if (FollowCamera) {
// 		return FollowCamera->GetComponentLocation();
// 	}
// 
// 	return Super::GetPawnViewLocation();
// }

void ASoldier::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ASoldier::StartFire);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &ASoldier::StopFire);

	PlayerInputComponent->BindAction("ADS", IE_Pressed, this, &ASoldier::ADS);
	PlayerInputComponent->BindAction("ADS", IE_Released, this, &ASoldier::StopADS);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ASoldier::ReloadCurrentGun);

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
}
