// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Equipment/Lethal.h"
#include "Entities/Soldier.h"

ALethal::ALethal()
{

}

void ALethal::UseEquipment(class ASoldier* Player)
{
	/** Validate player */
	if (Player) {

		OnUseEquipment(Player);
	}
}
