// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipableItem.h"
#include "Armor.generated.h"

UENUM(BlueprintType)
enum struct EArmorType : uint8
{
	Head,
	Body,
	Arms,
	Hands,
	Legs,
	Feet
};

UCLASS(Abstract, Blueprintable, BlueprintType, EditinlineNew)
class SAMPLE_API UArmor : public UEquipableItem
{
	GENERATED_BODY()

public:

	UArmor();

	/** Returns the armor type */
	FORCEINLINE EArmorType GetArmorType() const { return ArmorType; }

protected:

	/** Type of Armor */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Armor")
	EArmorType ArmorType;
};
