// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Equipment/Lethal.h"
#include "Entities/Soldier.h"
#include "StatAttributes/StatAttributeModifier.h"

ALethal::ALethal()
{
	Tag = "Health";
	Damage = 500;
}

void ALethal::UseEquipment(class ASoldier* Player)
{
	/** Validate player */
	if (Player) {

		OnUseEquipment(Player);
	}
}

class UStatAttributeModifier* ALethal::GetStatAttributeModifier()
{
	return UStatAttributeModifier::CreateModifier(Damage, EModificationType::MODTYPE_INSTANT_SINGLE, EOperationType::OPTYPE_SUBTRACT, Tag);
}
