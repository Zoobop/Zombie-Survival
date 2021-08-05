// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Equipment/Equipment.h"
#include <Net/UnrealNetwork.h>

AEquipment::AEquipment()
{
	Amount = 2;
	MaxAmount = 4;
}

void AEquipment::RefillEquipment()
{
	Amount = MaxAmount;
}

void AEquipment::ServerRefillEquipment_Implementation()
{
	RefillEquipment();
}

void AEquipment::AdjustEquipmentAmount(int32 AmountModification)
{
	Amount += AmountModification;

	if (Amount > MaxAmount) {
		Amount = MaxAmount;
	}
	else if (Amount < 0) {
		Amount = 0;
	}
}

bool AEquipment::CanUseEquipment() const
{
	/** Check amount */
	if (Amount > 0) {
		return true;
	}
	return false;
}

void AEquipment::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEquipment, Amount);
	DOREPLIFETIME(AEquipment, MaxAmount);
}
