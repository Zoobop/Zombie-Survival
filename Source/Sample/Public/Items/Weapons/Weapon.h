// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipableItem.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum struct EWeaponType : uint8
{
	Melee,
	Pistol,
	SMG,
	Rifle,
};

UCLASS(Abstract, Blueprintable, BlueprintType, EditinlineNew)
class SAMPLE_API UWeapon : public UEquipableItem
{
	GENERATED_BODY()

public:

	/** Call this function to use the weapon however it is defined to do so */
	UFUNCTION(BlueprintCallable)
	virtual void Action(FTransform SpawnTransform, class AEntity* Character) PURE_VIRTUAL(UWeapon, );

	/** Call this function to prep the weapon for use if needed */
	virtual void PreLoad() PURE_VIRTUAL(UWeapon, );

	/** Returns the weapon type */
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

protected:

	/** Type of Weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponType WeaponType;
};
