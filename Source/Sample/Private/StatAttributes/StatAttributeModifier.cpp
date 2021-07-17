// Fill out your copyright notice in the Description page of Project Settings.


#include "StatAttributes/StatAttributeModifier.h"
#include "StatAttributes/StatAttribute.h"

void UStatAttributeModifier::ApplyModification(class UStatAttribute* StatAttribute)
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

		/**  Modifies the bonus value */
		if (OperationType == EOperationType::OPTYPE_ADD) {
			StatAttribute->ModifyBonusValue(ModificationAmount);
		}
		else {
			StatAttribute->ModifyBonusValue(-ModificationAmount);
		}
		return;

	case EModificationType::MODTYPE_TICK_INFINTE:
		break;
	case EModificationType::MODTYPE_TICK_TIMER:
		break;
	default:
		break;
	}
}
