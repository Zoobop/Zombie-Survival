// Fill out your copyright notice in the Description page of Project Settings.


#include "StatAttributes/StatAttributeSet.h"
#include "StatAttributes/StatAttribute.h"


void UStatAttributeSet::AddModifiers(TArray<class UStatAttributeModifier*> Modifiers)
{
	StatAttributeModifiers.Append(Modifiers);
}

void UStatAttributeSet::ChangeModifier(class UStatAttributeModifier* Modifier, int32 Index)
{
	StatAttributeModifiers.Insert(Modifier, Index);
}

void UStatAttributeSet::ClearModifiers()
{
	StatAttributeModifiers.Empty();
}

void UStatAttributeSet::ClearModifier(class UStatAttributeModifier* Modifier)
{
	StatAttributeModifiers.RemoveSingle(Modifier);
}
