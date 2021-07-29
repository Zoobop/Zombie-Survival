// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/EntityStatComponent.h"
#include "Entities/Undead.h"
#include "Entities/Soldier.h"
#include "Entities/EntityState.h"
#include "StatAttributes/StatAttributeSet.h"
#include "StatAttributes/StatAttribute.h"
#include "StatAttributes/StatAttributeModifier.h"
#include "Net/UnrealNetwork.h"
#include "Gamemodes/SurvivalGameState.h"
#include "Gamemodes/SurvivalMode.h"

// Sets default values for this component's properties
UEntityStatComponent::UEntityStatComponent()
{
	/** Set defaults */
	Name = FText::FromString("New Entity");
	Faction = EFaction::FACTION_NONE;

	if (StatAttributeSet) {
		PresetStatAttributes();
	}
}

void UEntityStatComponent::SoldierDefaults()
{
	Name = FText::FromString("New Soldier");
	Faction = EFaction::FACTION_SURVIVORS;
}

void UEntityStatComponent::UndeadDefaults()
{
	Name = FText::FromString("New Undead");
	Faction = EFaction::FACTION_UNDEAD;
}

void UEntityStatComponent::ReceiveStatAttributeModification(TArray<class UStatAttributeModifier*> Modifiers)
{
	/** Validate Stat Attribute Set and add all the modifiers to the Stat Attribute Set */
	if (StatAttributeSet) {
		StatAttributeSet->AddModifiers(Modifiers);

		/** Search for matching attribute tags */
		for (UStatAttribute* StatAttribute : StatAttributeSet->GetStatAttributes()) {

			for (UStatAttributeModifier* Modifier : StatAttributeSet->GetStatAttributeModifiers()) {

				if (Modifier->GetStatAttributeTag() == StatAttribute->GetStatAttributeTag()) {
					/** Apply the modifier */
					Modifier->ApplyModification(StatAttribute);

					/** Check for the attribute death flags */
					CheckForDeath(StatAttribute);
				}
			}

			/** Update the Attribute changes */
			OnStatAttributeChangedDebug.Broadcast(StatAttribute);
		}

		/** Manage modifiers */
		ModifierMaintenance();
	}
}

void UEntityStatComponent::ModifierMaintenance()
{
	/** Manage modifiers based on their modification type */
	for (auto Modifier : StatAttributeSet->GetStatAttributeModifiers()) {

		if (Modifier->GetModificationType() == EModificationType::MODTYPE_INSTANT_SINGLE) {
			StatAttributeSet->ClearModifier(Modifier);
		}
	}
}

class UStatAttribute* UEntityStatComponent::FindStatAttribute(FName Tag) const
{
	/** Validate Stat Attribute Set */
	if (StatAttributeSet) {

		/** Search for the first instance of a stat attribute with the specified tag */
		for (auto Attribute : StatAttributeSet->GetStatAttributes()) {
			if (Attribute->GetStatAttributeTag() == Tag)
				return Attribute;
		}
	}
	return nullptr;
}

void UEntityStatComponent::SetDamageDealer(class AEntity* Attacker)
{
	if (Attacker) {
		DamageDealer = Attacker;
	}
}

void UEntityStatComponent::OnRep_Killer()
{
	OnHandleDeath.Broadcast(Killer);
	AEntity* EntityOwner = Cast<AEntity>(GetOwner());
	EntityOwner->Ragdoll();
	Killer->OnEntityKilled();
	//Killer->SendDeathData(EntityOwner);
}

void UEntityStatComponent::HandleDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Entity has died!"))
	if (DamageDealer) {
		Killer = DamageDealer;
		OnRep_Killer();
	}
}

bool UEntityStatComponent::CheckForDeath(class UStatAttribute* AssociatedStatAttribute)
{
	/** Validate stat attribute */
	if (AssociatedStatAttribute) {

		/** Check if below the threshold */
		if (AssociatedStatAttribute->GetCurrentValue() <= AssociatedStatAttribute->GetMinValue()) {

			/** Handle death */
			HandleDeath();
			return true;
		}
	}

	return false;
}

void UEntityStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEntityStatComponent, Killer);
	DOREPLIFETIME(UEntityStatComponent, DamageDealer);
}

// Called when the game starts
void UEntityStatComponent::BeginPlay()
{

	Super::BeginPlay();

	if (StatAttributeSet) {
		PresetStatAttributes();
		UE_LOG(LogTemp, Warning, TEXT("Vaild attribute set!"))
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Attribute set not found!"))

	OnStatAttributeChanged.Broadcast();
}

void UEntityStatComponent::PresetStatAttributes()
{
 	for (UStatAttribute* StatAttribute : StatAttributeSet->GetStatAttributes()) {
 		StatAttribute->SetOwningStatAttributeSet(StatAttributeSet);
    }
}

