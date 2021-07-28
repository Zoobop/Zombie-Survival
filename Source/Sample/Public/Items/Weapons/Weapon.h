// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipableItem.h"
#include "Interfaces/ESSModifierApplicationInterface.h"
#include "Interfaces/ESSModifierHandlerInterface.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum struct EWeaponType : uint8
{
	WPTYPE_MELEE	UMETA(DisplayName = "Melee"),
	WPTYPE_GUN	UMETA(DisplayName = "Gun"),
};

UCLASS(Abstract, Blueprintable, BlueprintType)
class SAMPLE_API AWeapon : public AEquipableItem, public IESSModifierHandlerInterface, public IESSModifierApplicationInterface
{
	GENERATED_BODY()

public:

	AWeapon();

	/** Call this function to use the weapon however it is defined to do so */
	UFUNCTION(BlueprintCallable)
	virtual void Action(FTransform SpawnTransform, class AEntity* Character) PURE_VIRTUAL(AWeapon, );

	/** Call this function to prep the weapon for use if needed */
	UFUNCTION(BlueprintCallable)
	virtual void LoadBullets() PURE_VIRTUAL(AWeapon, );

	/** Call to apply stat attribute modification */
	TArray<class UStatAttributeModifier*> ApplyStatAttributeModification() override;

	/** Allows for obtaining stat attribute modifiers through interface */
	class UStatAttributeModifier* GetStatAttributeModifier() override;

	/** Returns the weapon type */
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

protected:

	/** Scene component to hold the mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class USceneComponent* WeaponBase;

	/** Scene component to hold the mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class UStaticMeshComponent* HoldingPoint;

	/** Type of Weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponType WeaponType;
};
