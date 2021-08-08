// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Equipment/Equipment.h"
#include "Interfaces/ESSModifierHandlerInterface.h"
#include "Lethal.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API ALethal : public AEquipment, public IESSModifierHandlerInterface
{
	GENERATED_BODY()
	
public:

	ALethal();

	virtual void UseEquipment(class ASoldier* Player) override;

	UFUNCTION(BlueprintPure)
	class UStatAttributeModifier* GetStatAttributeModifier() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment|Lethal")
	int32 Damage;

};
