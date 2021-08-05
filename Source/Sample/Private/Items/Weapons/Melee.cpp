// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Melee.h"
#include "StatAttributes/StatAttributeModifier.h"

AMelee::AMelee()
{
	WeaponType = EWeaponType::WPTYPE_MELEE;

	bReplicates = true;
	SetReplicateMovement(true);
}

TArray<class UStatAttributeModifier*> AMelee::ApplyStatAttributeModification()
{
	return { GetStatAttributeModifier() };
}

class UStatAttributeModifier* AMelee::GetStatAttributeModifier()
{
	UStatAttributeModifier* Modifier = NewObject<UStatAttributeModifier>();
	Modifier->SetupModifier(DefaultDamage, EModificationType::MODTYPE_INSTANT_SINGLE, EOperationType::OPTYPE_SUBTRACT, Tag);
	return Modifier;
}
