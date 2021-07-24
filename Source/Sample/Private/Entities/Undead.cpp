
// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Undead.h"
#include "Entities/EntityStatComponent.h"

AUndead::AUndead()
{
	GetEntityStatComponent()->UndeadDefaults();
}

TArray<class UStatAttributeModifier*> AUndead::ApplyStatAttributeModification()
{
	TArray<class UStatAttributeModifier*> Modifiers;

	Modifiers.Add(StatAttributeModifier);

	return Modifiers;
}

