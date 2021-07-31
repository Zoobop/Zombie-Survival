// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PerkStations/StrongManPerkStation.h"
#include "Entities/Soldier.h"
#include "Entities/EntityState.h"
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
		if (AEntityState* EntityState = Cast<AEntityState>(Player->GetPlayerState())) {

			/** Search for perk modifiers */
			for (auto GameplayModifier : EntityState->GetGameplayModifiers()) {
				for (auto PerkModifier : PerkModifiers) {
					if (GameplayModifier == PerkModifier) {
						return true;
					}
				}
			}
		}
	}
	return false;
}