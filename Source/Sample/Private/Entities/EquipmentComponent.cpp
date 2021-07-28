// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/EquipmentComponent.h"
#include "Entities/Soldier.h"
#include "Items/Weapons/Gun.h"
#include "Items/Weapons/Melee.h"
#include "Items/Armor/Armor.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	WeaponIndex = 0;

	Player = Cast<ASoldier>(GetOwner());
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
  
bool UEquipmentComponent::AddGun(TSubclassOf<class AGun> GunToAdd)
{
	if (!Player->HasAuthority()) {
		ServerAddGun(GunToAdd);
		return true;
	}
	else {
		/** Validate gun to add */
		if (GunToAdd) {

			/** Check if there is enough space for more weapons */
			if (EquippedWeapons.Num() < WeaponCapacity) {

				/** Spawn in weapon and add to equipped weapons */
				if (AGun* NewGun = HandleWeaponSpawn(GunToAdd)) {
					EquippedWeapons.Add(NewGun);

					WeaponIndex = EquippedWeapons.Find(NewGun);
					OnRep_WeaponIndexChanged();
				}
			}
			else {
				/** Replace with current weapon if full */
				AGun* NewGun = HandleWeaponSpawn(GunToAdd);
				AGun* OldGun = EquippedWeapons[WeaponIndex];
				EquippedWeapons[WeaponIndex] = NewGun;

				OnWeaponSwitched.Broadcast(NewGun);
			}
			return true;
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

void UEquipmentComponent::PrepWeaponSwap(int32 ValueChange)
{
	/** Modify weapon index */
	int32 TempIndex = WeaponIndex;
	TempIndex += ValueChange;

	/** Clamp index */
	if (TempIndex < 0)
		TempIndex = EquippedWeapons.Num() - 1;
	else if (TempIndex > EquippedWeapons.Num() - 1)
		TempIndex = 0;

	WeaponIndex = TempIndex;
	OnRep_WeaponIndexChanged();
}

void UEquipmentComponent::SwitchOutWeapon(class AGun* WeaponToSwitchTo)
{
	if (!Player->HasAuthority()) {
		ServerSwitchWeapon(WeaponToSwitchTo);
	}
	else {
		/** Validate weapon to switch to */
		if (WeaponToSwitchTo) {
			if (CurrentWeapon) {
				CurrentWeapon->SetActorHiddenInGame(true);
			}
			CurrentWeapon = WeaponToSwitchTo;
			OnRep_WeaponChanged();
		}
	}
}

void UEquipmentComponent::ServerSwitchWeapon_Implementation(class AGun* WeaponToSwitchTo)
{
	/** Validate weapon to switch to */
	if (WeaponToSwitchTo) {
		if (CurrentWeapon) {
			CurrentWeapon->SetActorHiddenInGame(true);
		}
		CurrentWeapon = WeaponToSwitchTo;
		OnRep_WeaponChanged();
	}
}

void UEquipmentComponent::ServerAddGun_Implementation(TSubclassOf<class AGun> GunToAdd)
{
	/** Validate gun to add */
	if (GunToAdd) {

		/** Check if there is enough space for more weapons */
		if (EquippedWeapons.Num() < WeaponCapacity) {

			/** Spawn in weapon and add to equipped weapons */
			AGun* NewGun = HandleWeaponSpawn(GunToAdd);
			EquippedWeapons.Add(NewGun);

			WeaponIndex = EquippedWeapons.Find(NewGun);
			OnRep_WeaponIndexChanged();
		}
		else {
			/** Replace with current weapon if full */
			AGun* NewGun = HandleWeaponSpawn(GunToAdd);
			AGun* OldGun = EquippedWeapons[WeaponIndex];
			EquippedWeapons[WeaponIndex] = NewGun;

			OnWeaponSwitched.Broadcast(NewGun);
		}
	}
}

void UEquipmentComponent::SetMeleeVisibility(bool state)
{
	CurrentMelee->SetActorHiddenInGame(state);
}

////////////////////////////** --------------- Initialization --------------- **//////////////////////////////////

void UEquipmentComponent::BeginPlay()
{
 	Super::BeginPlay();
	
	InitializeEquipment();

}

class AGun* UEquipmentComponent::HandleWeaponSpawn(TSubclassOf<class AGun> Gun)
{
	if (Gun) {

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

class AMelee* UEquipmentComponent::HandleMeleeSpawn(TSubclassOf<class AMelee> Melee)
{
	if (Melee) {

		if (Player) {

			FTransform MeleeHolderSocket = Player->GetMeleeHolder()->GetComponentTransform();
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Player;
			SpawnParams.Instigator = Player;

			AMelee* SpawnMelee = GetWorld()->SpawnActor<AMelee>(Melee, MeleeHolderSocket, SpawnParams);

			SpawnMelee->AttachToComponent(Player->GetMeleeHolder(), FAttachmentTransformRules::SnapToTargetIncludingScale);
			SpawnMelee->SetActorHiddenInGame(true);

			return SpawnMelee;
		}
	}
	return nullptr;
}

class AArmor* UEquipmentComponent::HandleArmorSpawn(TSubclassOf<class AArmor> Armor)
{
	if (Armor) {

		if (Player) {

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Player;
			SpawnParams.Instigator = Player;

			AArmor* SpawnArmor = GetWorld()->SpawnActor<AArmor>(Armor, Player->GetActorTransform(), SpawnParams);

			return SpawnArmor;
		}
	}
	return nullptr;
}

void UEquipmentComponent::InitializeEquipment()
{
	/** Validate default weapons */
	if (DefaultWeapons.Num() > 0) {
		/** Adds default weapons to equipped list */
		for (auto DefaultGun : DefaultWeapons) {
			AGun* Gun = HandleWeaponSpawn(DefaultGun);
			EquippedWeapons.Add(Gun);
		}
	}

	/** Validate default melee weapon and make it the child actor class of weapon actor */
	if (DefaultMelee) {
		CurrentMelee = HandleMeleeSpawn(DefaultMelee);
	}

	/** Validate default armor */
	if (DefaultArmor.Num() > 0) {

	}

	/** Validate default lethals */
	if (DefaultLethals.Num() > 0) {

	}

	/** Validate default tacticals */
	if (DefaultTacticals.Num() > 0) {

	}

	/** Validate default consumables */
	if (DefaultConsumables.Num() > 0) {

	}

	PrepWeaponSwap(WeaponIndex);
	OnEquipmentChanged.Broadcast();
	Player->OnPlayerInitialized();
}



void UEquipmentComponent::OnRep_WeaponIndexChanged()
{
	OnWeaponSwitched.Broadcast(EquippedWeapons[WeaponIndex]);
}

void UEquipmentComponent::OnRep_WeaponChanged()
{
	CurrentWeapon->SetActorHiddenInGame(false);
}

void UEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEquipmentComponent, CurrentWeapon);
	DOREPLIFETIME(UEquipmentComponent, CurrentMelee);
	DOREPLIFETIME(UEquipmentComponent, WeaponIndex);
}