// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipableItem.h"
#include "Equipment.generated.h"

UENUM(BlueprintType)
enum struct EquipmentType : uint8
{
	EQUIPMENT_CONSUMABLE		UMETA(DisplayName = "Consumable"),
	EQUIPMENT_LETHAL			UMETA(DisplayName = "Lethal"),
	EQUIPMENT_TACTICAL			UMETA(DisplayName = "Tactical"),
};


UCLASS(Abstract)
class SAMPLE_API AEquipment : public AEquipableItem
{
	GENERATED_BODY()
	
public:

	AEquipment();

	/** Fills the equipment to the max amount */
	UFUNCTION(BlueprintCallable)
	void RefillEquipment();

	/** Adds the specific number to the amount */
	UFUNCTION(BlueprintCallable)
	void AdjustEquipmentAmount(int32 AmountToAdd);

	/** Pure virtual function that uses the equipment specified by that equipment */
	virtual void UseEquipment() PURE_VIRTUAL(AEquipment, );

	UFUNCTION(BlueprintImplementableEvent)
	void OnUseEquipment();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Amount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	int32 MaxAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	class UTexture2D* Icon;
};
