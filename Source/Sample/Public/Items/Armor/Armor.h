// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipableItem.h"
#include "Armor.generated.h"

UENUM(BlueprintType)
enum struct EArmorType : uint8
{
	ARTYPE_HEAD		UMETA(DisplayName = "Head"),
	ARTYPE_TORSO	UMETA(DisplayName = "Torso"),
	ARTYPE_ARMS		UMETA(DisplayName = "Arms"),
	ARTYPE_LEGS		UMETA(DisplayName = "Legs"),
};

UCLASS(Abstract, Blueprintable, BlueprintType)
class SAMPLE_API AArmor : public AEquipableItem
{
	GENERATED_BODY()

public:

	AArmor();

	/** Returns the armor type */
	FORCEINLINE EArmorType GetArmorType() const { return ArmorType; }

protected:

	/** Type of Armor */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Armor")
	EArmorType ArmorType;
};
