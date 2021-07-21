// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/EquipmentComponent.h"
#include "Entities/Soldier.h"
#include "Items/Weapons/Gun.h"
#include "Items/Armor/Armor.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	WeaponIndex = 0;

  	DefaultArmor.Add(0, nullptr);
  	DefaultArmor.Add(1, nullptr);
  	DefaultArmor.Add(2, nullptr);
  	DefaultArmor.Add(3, nullptr);
}

bool UEquipmentComponent::AddToEquipSlot(class AEquipableItem* ItemToAdd)
{
 	/** Check if ItemToAdd is valid */
  	if (ItemToAdd) {
  
  		/** Check if the ItemToRemove is a Weapon or Armor */
  		if (ItemToAdd->GetItemType() == EEquipableType::EQTYPE_WEAPON && EquippedWeapons.Num() < WeaponCapacity) {
  			/** Cast item to Weapon and add */
  			EquippedWeapons.Add(Cast<AGun>(ItemToAdd));
  
  			/** Update Equipment UI */
  			OnEquipmentChanged.Broadcast();
  
  			/** Update Current Weapon Mesh */
  			OnWeaponSwitched.Broadcast(CurrentWeapon);
  		}
  		else if (ItemToAdd->GetItemType() == EEquipableType::EQTYPE_ARMOR && EquippedArmor.Num() < ArmorCapacity) {
  			/** Cast item to Armor */
  			const auto ArmorItem = Cast<AArmor>(ItemToAdd);
  
  			/** Remove the old item and drop or destroy */
  			uint8 Key = static_cast<uint8>(ArmorItem->GetArmorType());
  			const auto OldArmorItem = *EquippedArmor.Find(Key);
  			//Drop or destroy...
  
  			/** Add the new Armor to the Map */
  			EquippedArmor.Emplace(Key, ArmorItem);
  
  			/** Update Equipment UI */
  			OnEquipmentChanged.Broadcast();
  		}
  	}
  
  	return false;
}
  
bool UEquipmentComponent::RemoveFromEquipSlot(class AEquipableItem* ItemToRemove)
{
  	/** Check if ItemToRemove is valid */
  	if (ItemToRemove) {
  
  		/** Check if the ItemToRemove is a Weapon */
  		if (EquippedWeapons.Contains(ItemToRemove)) {
  			/** Cast item to Weapon */
  			const auto OldWeaponItem = Cast<AGun>(ItemToRemove);
  
  			/** Validate the old item */
  			if (OldWeaponItem) {
  				/** Remove old item and drop or destroy based on amount */
  				EquippedWeapons.RemoveSingle(OldWeaponItem);
  				//Drop or destroy...
  			}
  
  			/** Update Equipment UI */
  			OnEquipmentChanged.Broadcast();
  
  			/** Update Current Weapon Mesh -- in case it was changed */
  			OnWeaponSwitched.Broadcast(CurrentWeapon);
  			return true;
  		}
  
  		/** Check if the ItemToRemove is Armor */
  		if (ItemToRemove->GetItemType() == EEquipableType::EQTYPE_ARMOR) {
  			/** Cast item to Armor */
  			const auto ArmorItem = Cast<AArmor>(ItemToRemove);
  
  			/** Validate the old item */
  			if (ArmorItem) {
  				/** Remove the old item and drop or destroy */
  				uint8 Key = static_cast<uint8>(ArmorItem->GetArmorType());
  				const auto OldArmorItem = *EquippedArmor.Find(Key);
  				// Drop or destroy...
  
  				/** Clear the value from the Map */
  				EquippedArmor.Emplace(Key, nullptr);
  			}
  
  			/** Update Equipment UI */
  			OnEquipmentChanged.Broadcast();
  			return true;
  		}
  	}
  
  	return false;
}

void UEquipmentComponent::ServerPrepWeaponSwap_Implementation(int32 ValueChange)
{
	PrepWeaponSwap(ValueChange);
}

void UEquipmentComponent::PrepWeaponSwap(int32 ValueChange)
{
	/** Modify weapon index */
	WeaponIndex += ValueChange;

	/** Clamp index */
	if (WeaponIndex < 0)
		WeaponIndex = EquippedWeapons.Num() - 1;
	else if (WeaponIndex > EquippedWeapons.Num() - 1)
		WeaponIndex = 0;

	/** Update weapon */
	OnWeaponSwitched.Broadcast(EquippedWeapons[WeaponIndex]);
}

void UEquipmentComponent::SwitchOutWeapon(class AGun* WeaponToSwitchTo)
{
	/** Validate current weapon and weapon to switch to */
	if (CurrentWeapon && WeaponToSwitchTo) {
		/** Hide weapon that is being switched out */
		CurrentWeapon->SetActorHiddenInGame(true);

		/** Get weapon from index and assign to current weapon -- Set actor to be visible */
		CurrentWeapon = WeaponToSwitchTo;
		CurrentWeapon->SetActorHiddenInGame(false);
	}
}

void UEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEquipmentComponent, CurrentWeapon);
	DOREPLIFETIME(UEquipmentComponent, WeaponIndex);
}

class AGun* UEquipmentComponent::HandleWeaponSpawn(TSubclassOf<class AGun> Gun)
{
	if (Gun) {

		ASoldier* Player = Cast<ASoldier>(GetOwner());

		if (Player) {

			FTransform WeaponHolderSocket = Player->GetWeaponHolder()->GetComponentTransform();
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Player;
			SpawnParams.Instigator = Player;

			AGun* SpawnedGun = GetWorld()->SpawnActor<AGun>(Gun, WeaponHolderSocket, SpawnParams);
			SpawnedGun->LoadBullets();
			SpawnedGun->SetGunOwner(Player);

			SpawnedGun->AttachToComponent(Player->GetWeaponHolder(), FAttachmentTransformRules::SnapToTargetIncludingScale);
			SpawnedGun->SetActorHiddenInGame(true);
			return SpawnedGun;
		}
	}
	return nullptr;
}

// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
 	Super::BeginPlay();
	
	WeaponSetup();
}

void UEquipmentComponent::WeaponSetup()
{
	for (auto DefaultGun : DefaultWeapons) {
		AGun* Gun = HandleWeaponSpawn(DefaultGun);
		EquippedWeapons.Add(Gun);
	}

	/** Assign the current weapon to be the first weapon in weapons list */
	if (EquippedWeapons[0]) {
		CurrentWeapon = EquippedWeapons[0];
		CurrentWeapon->SetActorHiddenInGame(false);

		/** Update current weapon mesh */
		OnEquipmentChanged.Broadcast();
	}
}
