// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/Weapon.h"
#include "Melee.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API AMelee : public AWeapon
{
	GENERATED_BODY()

public:

	AMelee();
	
	/** Call to apply stat attribute modification */
	TArray<class UStatAttributeModifier*> ApplyStatAttributeModification() override;

	/** Allows for obtaining stat attribute modifiers through interface */
	class UStatAttributeModifier* GetStatAttributeModifier() override;

protected:

	/** Base weapon Stat Modifier */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee|StatModifiers")
	TArray<class UStatAttributeModifier*> StatAttributeModifiers;

	/** Modifier Tag */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee|StatModifiers")
	FName Tag = "Health";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	int32 DefaultDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	float DefaultAttackSpeed;
};
