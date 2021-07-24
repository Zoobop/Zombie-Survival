// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StatAttributeModifier.generated.h"

UENUM(BlueprintType)
enum struct EModificationType : uint8
{
	MODTYPE_INSTANT_SINGLE		UMETA(DisplayName = "Instant Single"),
	MODTYPE_INSTANT_INFINTE		UMETA(DisplayName = "Instant Infinite"),
	MODTYPE_TICK_INFINTE		UMETA(DisplayName = "Tick Infinite"),
	MODTYPE_TICK_TIMER			UMETA(DisplayName = "Tick Timer"),
};

UENUM(BlueprintType)
enum struct EOperationType : uint8
{
	OPTYPE_ADD			UMETA(DisplayName = "Additive"),
	OPTYPE_SUBTRACT		UMETA(DisplayName = "Subtractive"),
};


UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class SAMPLE_API UStatAttributeModifier : public UDataAsset
{
	GENERATED_BODY()

public:
	UStatAttributeModifier();

	void ApplyModification(class UStatAttribute* StatAttribute);

	void SetupModifier(int32 Amount, EModificationType MODType, EOperationType OPType, FName Tag);

	UFUNCTION(BlueprintImplementableEvent)
	void OnApplyModification();

	/** Returns the Stat Attribute Tag */
	FORCEINLINE FName GetStatAttributeTag() const { return StatAttributeTag; }
	/** Returns the modification amount */
	FORCEINLINE int32 GetModificationAmount() const { return ModificationAmount; }
	/** Returns the modification type */
	FORCEINLINE EModificationType GetModificationType() const { return ModificationType; }
	/** Returns the operation type */
	FORCEINLINE EOperationType GetOperationType() const { return OperationType; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatModification")
	int32 ModificationAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatModification")
	EModificationType ModificationType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatModification")
	EOperationType OperationType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatModification")
	bool bAppliesTick;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatModification", meta = (EditCondition = "bAppliesTick"))
	float TickTimerLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatModification|Tags")
	FName StatAttributeTag;

	FTimerHandle TimerHandle_TickModification;

};
