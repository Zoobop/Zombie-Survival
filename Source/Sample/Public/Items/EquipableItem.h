// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EquipableItem.generated.h"

UENUM(BlueprintType)
enum struct EEquipableType : uint8
{
	Weapon,
	Armor,
	Consumable
};

UCLASS(Abstract, Blueprintable, BlueprintType, EditinlineNew, DefaultToInstanced)
class SAMPLE_API UEquipableItem : public UObject
{
	GENERATED_BODY()

		/** ------------- Functions ------------- **/
public:

	UEquipableItem();

	/** Use the item */
	virtual void Use(class AEntity* Character) PURE_VIRTUAL(UEquipableItem, );

	/** Use the item (Blueprint) */
	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(class AEntity* Entity);

	/** Returns the equipment that the equipable item belongs to */
	FORCEINLINE class UEquipmentComponent* GetOwningEquipment() const { return OwningEquipment; }
	/** Returns the mesh of the equipable item */
	FORCEINLINE EEquipableType GetItemType() const { return Type; }
	/** Returns the mesh of the equipable item */
	FORCEINLINE class USkeletalMesh* GetSkeletalMesh() const { return SkeletalMesh; }

	/** --------------- Fields --------------- **/
protected:

	UPROPERTY(VisibleInstanceOnly, Category = "Item")
	class UEquipmentComponent* OwningEquipment;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class USkeletalMesh* SkeletalMesh;

	/** Item Thumbnail */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	class UTexture2D* Thumbnail;

	/** Item Pickup Text */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FText ItemPickupText;

	/** Item Name */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FText ItemDisplayName;

	/** Item Description */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (MultiLine = true))
	FText ItemDescription;

	/** Item Type */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	EEquipableType Type;
};
