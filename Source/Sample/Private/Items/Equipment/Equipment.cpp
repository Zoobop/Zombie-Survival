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

void AEquipment::AdjustEquipmentAmount(int32 AmountModification)
{
	Amount += AmountModification;

	if (Amount < MaxAmount) {
		Amount = MaxAmount;
	}
	else if (Amount > 0) {
		Amount = 0;
	}
}

void AEquipment::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEquipment, Amount);
	DOREPLIFETIME(AEquipment, MaxAmount);
}
