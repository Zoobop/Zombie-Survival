// Fill out your copyright notice in the Description page of Project Settings.


#include "StatAttributes/StatAttributeModifier.h"
#include "StatAttributes/StatAttribute.h"


UStatAttributeModifier::UStatAttributeModifier()
{

}

void UStatAttributeModifier::ApplyModification(class UStatAttribute* StatAttribute, UWorld* EntityWorld)
{
	switch (ModificationType)
	{
		/** Instant action/effect for a single instance -- i.e. Zombie attack, standard-issued bullet damage, med packs */
	case EModificationType::MODTYPE_INSTANT_SINGLE:

		/** Modifies the current value */
		if (OperationType == EOperationType::OPTYPE_ADD) {
			StatAttribute->ModifyCurrentValue(ModificationAmount);
		}
		else {
			StatAttribute->ModifyCurrentValue(-ModificationAmount);
		}
		return;

		/** Instant action/effect for an indefinite time -- i.e. Gear and perk upgrades, debuffs that require action to rid */
	case EModificationType::MODTYPE_INSTANT_INFINTE:

		/** Modifies the bonus value */
		if (OperationType == EOperationType::OPTYPE_ADD) {
			StatAttribute->ModifyBonusValue(ModificationAmount);
		}
		else {
			StatAttribute->ModifyBonusValue(-ModificationAmount);
		}
		return;

	case EModificationType::MODTYPE_TICK_INFINTE:

		/** Modifies the current value per tick */
		SetCurrentAttribute(StatAttribute, EntityWorld);

		return;

	case EModificationType::MODTYPE_TICK_TIMER:
		return;
	default:
		break;
	}
}

void UStatAttributeModifier::SetupModifier(int32 Amount, EModificationType MODType, EOperationType OPType, FName Tag)
{
	ModificationAmount = Amount;
	ModificationType = MODType;
	OperationType = OPType;
	StatAttributeTag = Tag;
}

void UStatAttributeModifier::SetCurrentAttribute(class UStatAttribute* Current, UWorld* EntityWorld)
{
	/** Validate current */
	if (Current) {

		CurrentAttribute = Current;
		GameWorld = EntityWorld;

		bHasFinishedTicking = false;
	}
}

int32 UStatAttributeModifier::CompareByTag(UStatAttributeModifier* Other)
{
	/** Check if has valid tag */
	if (GetStatModifierTag() == "None") {
	}
	/** Validate other */
	else if (Other) {

		/** Compare */
		if (GetStatModifierTag() != Other->GetStatModifierTag()) {
			return -1;
		}
		else {
			/** Tags are equal */
			return 0;
		}

	}
	return -1;
}

bool UStatAttributeModifier::CompareByValue(UStatAttributeModifier* Other)
{
	/** Validate other */
	if (Other) {

		/** Compare */
		if (GetModificationAmount() > Other->GetModificationAmount()) {
			return true;
		}

	}
	return false;
}

void UStatAttributeModifier::StartTickTimer()
{
	if (!TimerHandle_TickModification.IsValid())
		GameWorld->GetTimerManager().SetTimer(TimerHandle_TickModification, this, &UStatAttributeModifier::ModificationTick, TickTimerLength, true, TickTimerStart);
}

void UStatAttributeModifier::ModificationTick()
{
	/** Validate current attribute */
	if (CurrentAttribute) {

		/** Check if the current value has reach the max current value */
		if (CurrentAttribute->GetCurrentValue() >= CurrentAttribute->GetMaxCurrentValue()) {
			ClearTickTimer();
		}
		else {
			if (OperationType == EOperationType::OPTYPE_ADD) {
				CurrentAttribute->ModifyCurrentValue(ModificationAmount);
			}
			else {
				CurrentAttribute->ModifyCurrentValue(-ModificationAmount);
			}
		}
	}
}

void UStatAttributeModifier::ClearTickTimer()
{
	if (TimerHandle_TickModification.IsValid()) {
		GameWorld->GetTimerManager().ClearTimer(TimerHandle_TickModification);
		bHasFinishedTicking = true;
	}
}
