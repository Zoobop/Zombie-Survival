// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatAttribute.generated.h"


UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class SAMPLE_API UStatAttribute : public UObject
{
	GENERATED_BODY()
	
public:

	UStatAttribute();
	
	/** Refill the attributes */
	void RefillAttributes();

	/** Allows for editing the values */
	void SetValues(int32 Default, int32 Current, int32 Bonus);

	/** Set the values to the default values */
	void ResetToDefault();

	/** Set the current value */
	void ModifyCurrentValue(int32 Value);

	/** Set the bonus value */
	void ModifyBonusValue(int32 Value);

	/** Set the infinite bonus value */
	void ModifyInfinteBonusValue(int32 Value);


	/** Set Owner */
	void SetOwningStatAttributeSet(class UStatAttributeSet* StatAttributeSet);

	/** Returns the stat attribute tag of the stat attribute */
	FORCEINLINE FName GetStatAttributeTag() const { return StatAttributeTag; }
	/** Returns the current value of the stat attribute */
	FORCEINLINE int32 GetCurrentValue() const { return CurrentValue; }
	/** Returns the min current value of the stat attribute */
	FORCEINLINE int32 GetMinValue() const { return MinValue; }
	/** Returns the max current value of the stat attribute */
	FORCEINLINE int32 GetMaxCurrentValue() const { return DefaultValue + BonusValue + InfiniteBonusValue; }
	/** Returns if the current value is equal to the max value */
	FORCEINLINE bool HasFullValue() const { return CurrentValue == DefaultValue + BonusValue + InfiniteBonusValue; }

protected:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	class UStatAttributeSet* OwningStatAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "StatAttribute", meta = (ClampMin = 0))
	int32 DefaultValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "StatAttribute", meta = (ClampMin = 0))
	int32 CurrentValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatAttribute")
	int32 MinValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatAttribute", meta = (EditCondition = "bHasMaxValue"))
	int32 MaxValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatAttribute")
	bool bHasMaxValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "StatAttribute")
	int32 BonusValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "StatAttribute")
	int32 InfiniteBonusValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatAttribute|Flags")
	bool bChecksForDeath;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatAttribute|Tags")
	FName StatAttributeTag;

private:

	float StartTime;

	float TimeTickValue;

	int32 ModifierValue;
};
