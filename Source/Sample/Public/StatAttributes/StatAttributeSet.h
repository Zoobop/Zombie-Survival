// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatAttributeSet.generated.h"

UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class SAMPLE_API UStatAttributeSet : public UObject
{
	GENERATED_BODY()
	
public:

	/** Adds modifiers to the modifiers list */
	void AddModifiers(TArray<class UStatAttributeModifier*> Modifiers);

	/** Clears all modifiers in the list */
	void ClearModifiers();
	
	/** Clears the specified modifier from the list */
	void ClearModifier(class UStatAttributeModifier* Modifier);

	FORCEINLINE TArray<class UStatAttribute*> GetStatAttributes() const { return StatAttributes; }
	FORCEINLINE TArray<class UStatAttributeModifier*> GetStatAttributeModifiers() const { return StatAttributeModifiers; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "StatAttribute")
	TArray<class UStatAttribute*> StatAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatAttribute")
	TArray<class UStatAttributeModifier*> StatAttributeModifiers;

};
