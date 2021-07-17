// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/EquipmentComponent.h"
#include "Items/Weapons/Gun.h"
#include "Items/Armor/Armor.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	WeaponIndex = 0;

  	DefaultArmor.Add(0, nullptr);
  	DefaultArmor.Add(1, nullptr);
  	DefaultArmor.Add(2, nullptr);
  	DefaultArmor.Add(3, nullptr);
  	DefaultArmor.Add(4, nullptr);
  	DefaultArmor.Add(5, nullptr);
}


bool UEquipmentComponent::AddToEquipSlot(class UEquipableItem* ItemToAdd)
{
 	/** Check if ItemToAdd is valid */
  	if (ItemToAdd) {
  
  		/** Check if the ItemToRemove is a Weapon or Armor */
  		if (ItemToAdd->GetItemType() == EEquipableType::Weapon && EquippedWeapons.Num() < WeaponCapacity) {
  			/** Cast item to Weapon and add */
  			EquippedWeapons.Add(static_cast<UGun*>(ItemToAdd));
  
  			/** Update Equipment UI */
  			OnEquipmentChanged.Broadcast();
  
  			/** Update Current Weapon Mesh */
  			OnWeaponSwitched.Broadcast();
  		}
  		else if (ItemToAdd->GetItemType() == EEquipableType::Armor && EquippedArmor.Num() < ArmorCapacity) {
  			/** Cast item to Armor */
  			const auto ArmorItem = static_cast<UArmor*>(ItemToAdd);
  
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
  
bool UEquipmentComponent::RemoveFromEquipSlot(class UEquipableItem* ItemToRemove)
{
  	/** Check if ItemToRemove is valid */
  	if (ItemToRemove) {
  
  		/** Check if the ItemToRemove is a Weapon */
  		if (EquippedWeapons.Contains(ItemToRemove)) {
  			/** Cast item to Weapon */
  			const auto OldWeaponItem = static_cast<UGun*>(ItemToRemove);
  
  			/** Validate the old item */
  			if (OldWeaponItem) {
  				/** Remove old item and drop or destroy based on amount */
  				EquippedWeapons.RemoveSingle(OldWeaponItem);
  				//Drop or destroy...
  			}
  
  			/** Update Equipment UI */
  			OnEquipmentChanged.Broadcast();
  
  			/** Update Current Weapon Mesh -- in case it was changed */
  			OnWeaponSwitched.Broadcast();
  			return true;
  		}
  
  		/** Check if the ItemToRemove is Armor */
  		if (ItemToRemove->GetItemType() == EEquipableType::Armor) {
  			/** Cast item to Armor */
  			const auto ArmorItem = static_cast<UArmor*>(ItemToRemove);
  
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

class USkeletalMesh* UEquipmentComponent::DisplayWeaponMesh()
{
 	/** Validate current weapon */
  	if (CurrentWeapon) {
  		return CurrentWeapon->GetSkeletalMesh();
  	}
 
 	return nullptr;
}

void UEquipmentComponent::SwitchWeapon(int32 ValueChange)
{
	/** Modify weapon index */
	WeaponIndex += ValueChange;

   	/** Clamp index */
   	if (WeaponIndex < 0)
   		WeaponIndex = EquippedWeapons.Num() - 1;
   	else if (WeaponIndex > EquippedWeapons.Num() - 1)
   		WeaponIndex = 0;
   
   	/** Get weapon from index and assign to current weapon */
   	CurrentWeapon = EquippedWeapons[WeaponIndex];
 
  	/** Update weapon mesh */
  	OnWeaponSwitched.Broadcast();
}
 
// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
 	Super::BeginPlay();

  	/** Add starting weapons to weapon equip slots */
 	EquippedWeapons.Append(DefaultWeapons);
  
  	/** Add starting armor to armor equip slots */
 	EquippedArmor.Append(DefaultArmor);
  
  	/** Assign the current weapon to be the first weapon in weapons list */
  	CurrentWeapon = EquippedWeapons[0];

  	/** Update current weapon mesh */
  	OnWeaponSwitched.Broadcast();
}
