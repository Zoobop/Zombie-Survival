// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Entity.h"
#include "Undead.generated.h"


UCLASS(BlueprintType)
class SAMPLE_API AUndead : public AEntity
{
	GENERATED_BODY()
	
public:
	AUndead();

	/** Call when receiving damage from another entity */
	void ReceiveStatAttributeModification(TArray<class UStatAttributeModifier*> Modifiers) override;

	/** Call when applying damage to another entity */
	TArray<class UStatAttributeModifier*> ApplyStatAttributeModification() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Undead")
	class UStatAttributeModifier* StatAttributeModifier;
};
