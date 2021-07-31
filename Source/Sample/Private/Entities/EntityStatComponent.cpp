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

void UEntityStatComponent::ApplyCurrentModifiers()
{
	/** Validate Stat Attribute Set */
	if (StatAttributeSet) {

		/** Search for matching attribute tags */
		for (UStatAttribute* StatAttribute : StatAttributeSet->GetStatAttributes()) {

			for (UStatAttributeModifier* Modifier : StatAttributeSet->GetStatAttributeModifiers()) {

				if (Modifier->GetStatAttributeTag() == StatAttribute->GetStatAttributeTag()) {
					/** Apply the modifier */
					Modifier->ApplyModification(StatAttribute, GetWorld());

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

void UEntityStatComponent::ReceiveStatAttributeModification(TArray<class UStatAttributeModifier*> Modifiers)
{
	/** Validate Stat Attribute Set and add all the modifiers to the Stat Attribute Set */
	if (StatAttributeSet) {
		CheckModifiers(Modifiers);

		ApplyCurrentModifiers();
	}
}

void UEntityStatComponent::CheckModifiers(TArray<class UStatAttributeModifier*> Modifiers)
{
	/** Search through modifiers for matching modifier tags */
	for (auto Modifier : StatAttributeSet->GetStatAttributeModifiers()) {
		for (auto NewModifier : Modifiers) {

			/** Compare modifiers */
			if (Modifier->CompareByTag(NewModifier) == 0) {
				StatAttributeSet->ClearModifier(Modifier);
			}

			StatAttributeSet->AddModifiers({ NewModifier });
		}
	}
}

void UEntityStatComponent::ModifierMaintenance()
{
	/** Manage modifiers based on their modification type */
	for (auto Modifier : StatAttributeSet->GetStatAttributeModifiers()) {

		if (Modifier->GetModificationType() == EModificationType::MODTYPE_INSTANT_SINGLE) {

			StatAttributeSet->ClearModifier(Modifier);
		}
		else if (Modifier->GetModificationType() == EModificationType::MODTYPE_INSTANT_INFINTE) {

			StatAttributeSet->ClearModifier(Modifier);

			if (AEntityState* EntityState = Cast<AEntityState>(Cast<AEntity>(GetOwner())->GetPlayerState())) {
				EntityState->AddModifiers({ Modifier });
			}
			
		}
		else if (Modifier->GetModificationType() == EModificationType::MODTYPE_TICK_INFINTE) {

			if (!Modifier->HasFinishedTick()) {
				Modifier->StartTickTimer();
			}

		}
	}

	OnStatAttributeChanged.Broadcast();
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
	}

	ApplyCurrentModifiers();
	OnStatAttributeChanged.Broadcast();
}

void UEntityStatComponent::PresetStatAttributes()
{
 	for (UStatAttribute* StatAttribute : StatAttributeSet->GetStatAttributes()) {
 		StatAttribute->SetOwningStatAttributeSet(StatAttributeSet);
    }
}

