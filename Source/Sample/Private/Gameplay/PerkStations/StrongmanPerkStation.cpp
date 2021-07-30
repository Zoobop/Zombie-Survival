// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PerkStations/StrongManPerkStation.h"
#include "Entities/EntityStatComponent.h"
#include "Entities/Soldier.h"
#include "StatAttributes/StatAttributeSet.h"
#include "StatAttributes/StatAttributeModifier.h"

AStrongManPerkStation::AStrongManPerkStation()
{
	PerkName = FText::FromString("Strong Man");
	PerkCost = 2500;
}

bool AStrongManPerkStation::HasPerk(class ASoldier* Player) const
{
	/** Validate player */
	if (Player) {

		/** Validate player entity stat component */
		if (UEntityStatComponent* EntityStatComponent = Player->GetEntityStatComponent()) {

			/** Search for perk modifiers */
			for (auto PlayerModifier : EntityStatComponent->GetStatAttributeSet()->GetStatAttributeModifiers()) {
				for (auto PerkModifier : PerkModifiers) {
					if (PlayerModifier == PerkModifier) {
						return true;
					}
				}
			}
		}
	}
	return false;
}