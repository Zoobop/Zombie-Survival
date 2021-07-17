
// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Undead.h"
#include "Entities/EntityStatComponent.h"

AUndead::AUndead()
{
	GetEntityStatComponent()->UndeadDefaults();
}

void AUndead::ReceiveStatAttributeModification(TArray<class UStatAttributeModifier*> Modifiers)
{
	GetEntityStatComponent()->ApplyStatAttributeModifiers(Modifiers);
}

TArray<class UStatAttributeModifier*> AUndead::ApplyStatAttributeModification()
{
	TArray<class UStatAttributeModifier*> Modifiers;

	Modifiers.Add(StatAttributeModifier);

	return Modifiers;
}

