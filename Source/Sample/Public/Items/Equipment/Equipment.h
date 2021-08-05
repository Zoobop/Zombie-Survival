// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipableItem.h"
#include "Equipment.generated.h"

UENUM(BlueprintType)
enum struct EEquipmentType : uint8
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

	/** Fills the equipment to the max amount */
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRefillEquipment();

	/** Adds the specific number to the amount */
	UFUNCTION(BlueprintCallable)
	void AdjustEquipmentAmount(int32 AmountToAdd);

	/** Checks if the item can be used */
	UFUNCTION(BlueprintCallable)
	bool CanUseEquipment() const;

	/** Pure virtual function that uses the equipment specified by that equipment */
	virtual void UseEquipment() PURE_VIRTUAL(AEquipment, );

	UFUNCTION(BlueprintImplementableEvent)
	void OnUseEquipment();

	/** Returns the current amount */
	FORCEINLINE int32 GetCurrentAmount() const { return Amount; }
	/** Returns the max amount */
	FORCEINLINE int32 GetMaxAmount() const { return MaxAmount; }

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment", Replicated)
	int32 Amount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment", Replicated)
	int32 MaxAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment", Replicated)
	class UTexture2D* Icon;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	EEquipmentType EquipmentType;
};
