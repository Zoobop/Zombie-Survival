// Fill out your copyright notice in the Description page of Project Settings.


#include "StatAttributes/StatAttribute.h"
#include "StatAttributes/StatAttributeSet.h"

UStatAttribute::UStatAttribute()
{
	BonusValue = 0;
	DefaultValue = 100 + BonusValue + InfiniteBonusValue;
	CurrentValue = DefaultValue;
}

void UStatAttribute::SetValues(int32 Default, int32 Current, int32 Bonus)
{
	DefaultValue = Default;
	CurrentValue = Current;
	BonusValue = Bonus;
}

void UStatAttribute::ResetToDefault()
{
	BonusValue = 0;
	DefaultValue = 100 + BonusValue + InfiniteBonusValue;
	CurrentValue = DefaultValue;
}

void UStatAttribute::ModifyCurrentValue(int32 Value)
{
	/** Add to current value */
	CurrentValue += Value;
	
	/** Check for min value */
	if (CurrentValue < MinValue)
		CurrentValue = MinValue;
	/** Check for current max value */
	else if (CurrentValue > GetMaxCurrentValue())
		CurrentValue = GetMaxCurrentValue();
	/** Check if has max value */
	else if (bHasMaxValue && CurrentValue > MaxValue)
		CurrentValue = MaxValue;
}

void UStatAttribute::ModifyBonusValue(int32 Value)
{
	BonusValue += Value;
}

void UStatAttribute::ModifyInfinteBonusValue(int32 Value)
{
	InfiniteBonusValue += Value;
}

void UStatAttribute::SetOwningStatAttributeSet(class UStatAttributeSet* StatAttributeSet)
{
	if (StatAttributeSet)
		OwningStatAttributeSet = StatAttributeSet;
}
