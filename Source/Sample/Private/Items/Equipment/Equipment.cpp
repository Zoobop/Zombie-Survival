// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Equipment/Equipment.h"

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
}
