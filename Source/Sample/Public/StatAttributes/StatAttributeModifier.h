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


	static UStatAttributeModifier* CreateModifier(int32 Amount, EModificationType MODType, EOperationType OPType, FName Tag);


	void ApplyModification(class UStatAttribute* StatAttribute, UWorld* EntityWorld);

	void SetupModifier(int32 Amount, EModificationType MODType, EOperationType OPType, FName Tag);

	/** Starts the timer for modifiers that apply tick modification */
	void StartTickTimer();

	/** Clears the tick timer */
	void ClearTickTimer();

	UFUNCTION(BlueprintImplementableEvent)
	void OnApplyModification();

	/** Compares the Stat Attribute Modifiers by Modifier Tag */
	int32 CompareByTag(UStatAttributeModifier* Other);

	/** Compares the Stat Attribute Modifiers by Modifier Values */
	bool CompareByValue(UStatAttributeModifier* Other);

	/** Returns the Stat Attribute Tag */
	FORCEINLINE FName GetStatAttributeTag() const { return StatAttributeTag; }
	/** Returns the Stat Modifier Tag */
	FORCEINLINE FName GetStatModifierTag() const { return StatModifierTag; }
	/** Returns the modification amount */
	FORCEINLINE int32 GetModificationAmount() const { return ModificationAmount; }
	/** Returns the modification type */
	FORCEINLINE EModificationType GetModificationType() const { return ModificationType; }
	/** Returns the operation type */
	FORCEINLINE EOperationType GetOperationType() const { return OperationType; }
	/** Returns if the modifier tick has finished */
	FORCEINLINE bool HasFinishedTick() const { return bHasFinishedTicking; }

protected:

	/** Stores in the stat attribute and world that is currently being modified  */
	void SetCurrentAttribute(class UStatAttribute* Current, UWorld* EntityWorld);

	/** Controls the application and exiting of tick timer */
	void ModificationTick();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatModification", meta = (EditCondition = "bAppliesTick"))
	float TickTimerStart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatModification|Tags")
	FName StatAttributeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatModification|Tags")
	FName StatModifierTag;

	FTimerHandle TimerHandle_TickModification;

private:

	class UStatAttribute* CurrentAttribute;

	class UWorld* GameWorld;

	bool bHasFinishedTicking;
};
