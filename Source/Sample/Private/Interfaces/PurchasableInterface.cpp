// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/PurchasableInterface.h"
#include "Entities/EntityState.h"
#include "Entities/Soldier.h"

// Add default functionality here for any IPurchasableInterface functions that are not pure virtual.

bool IPurchasableInterface::CheckPlayerPoints(class ASoldier* Player, int32 PointsNeeded) const
{
	/** Validate player */
	if (Player) {

		/** Validate and check points */
		if (AEntityState* State = Cast<AEntityState>(Player->GetPlayerState())) {
			if (State->GetCurrentPoints() >= PointsNeeded)
				return true;
		}
	}
	return false;
}

void IPurchasableInterface::RemovePointsFromPlayer(class ASoldier* Player, int32 PointsToRemove) const
{
	/** Validate player */
	if (Player) {
		/** Remove points */
		if (AEntityState* PlayerState = Cast<AEntityState>(Player->GetPlayerState())) {
			PlayerState->RemovePoints(PointsToRemove);
		}
	}
}
