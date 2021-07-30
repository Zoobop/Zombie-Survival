// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PerkStations/RecoveryExpertPerkStation.h"
#include "Entities/Soldier.h"
#include "Entities/EntityState.h"
#include "StatAttributes/StatAttributeSet.h"
#include "StatAttributes/StatAttributeModifier.h"

ARecoveryExpertPerkStation::ARecoveryExpertPerkStation()
{
	PerkName = FText::FromString("Recovery Expert");
	PerkCost = 1500;
}

bool ARecoveryExpertPerkStation::HasPerk(class ASoldier* Player) const
{
	/** Validate player */
	if (Player) {

		/** Validate player state */
		if (AEntityState* EntityState = Cast<AEntityState>(Player->GetPlayerState())) {
		
			/** Search for perk modifiers */
			for (auto GameplayModifier : EntityState->GetGameplayAttributes()->GetStatAttributeModifiers()) {
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
