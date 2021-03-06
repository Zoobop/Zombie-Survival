// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/WallBuys/WallBuy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Entities/Soldier.h"
#include "Items/Armor/Armor.h"
#include "Items/Equipment/Lethal.h"
#include "Items/Equipment/Tactical.h"
#include "Items/Equipment/Consumable.h"
#include "Entities/EquipmentComponent.h"
#include "Entities/EntityState.h"

// Sets default values
AWallBuy::AWallBuy()
{
	WallBuyBase = CreateDefaultSubobject<USkeletalMeshComponent>("ItemMesh");
	SetRootComponent(WallBuyBase);

	PurchaseTrigger = CreateDefaultSubobject<UBoxComponent>("PurchaseTrigger");
	PurchaseTrigger->SetupAttachment(WallBuyBase);

	WallBuyHighlight = CreateDefaultSubobject<UPointLightComponent>("WallBuyHighlight");
	WallBuyHighlight->SetupAttachment(WallBuyBase);
	WallBuyHighlight->SetSourceRadius(100.0f);
	WallBuyHighlight->SetSoftSourceRadius(200.0f);
	WallBuyHighlight->SetIntensity(100.0f);
}

void AWallBuy::Purchase(class ASoldier* Player)
{

}

// Called when the game starts or when spawned
void AWallBuy::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AWallBuy::CheckForItem(class ASoldier* Player)
{
	/** Validate player */
	if (Player) {

		UEquipmentComponent* Equipment = Player->GetEquipmentComponent();

		/** Check equipped armor */
		if (Equipment->GetEquippedArmor().Num() > 0){
			for (auto ArmorPair : Equipment->GetEquippedArmor()) {
				if (ArmorPair.Value->GetClass() == ItemToBuy)
					return true;
			}
		}

		/** Check equipped lethals */
		if (ALethal* Lethal = Equipment->GetEquippedLethal()) {
			if (Lethal->GetClass() == ItemToBuy)
				return true;
		}

		/** Check equipped tacticals */
		if (ATactical* Tactical = Equipment->GetEquippedTactical()) {
			if (Tactical->GetClass() == ItemToBuy)
				return true;
		}

	  	/** Check equipped consumables */
 		if (AConsumable* Consumable = Equipment->GetEquippedConsumable()) {
  			if (Consumable->GetClass() == ItemToBuy)
  				return true;
 		}

	}
	return false;
}
