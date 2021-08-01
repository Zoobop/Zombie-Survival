// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Entity.h"
#include "Interfaces/ESSModifierApplicationInterface.h"
#include "Undead.generated.h"


UCLASS(BlueprintType)
class SAMPLE_API AUndead : public AEntity, public IESSModifierApplicationInterface
{
	GENERATED_BODY()
	
public:
	AUndead();

	/** Call when applying damage to another entity */
	TArray<class UStatAttributeModifier*> ApplyStatAttributeModification() override;

protected:

	virtual void BeginPlay() override;

	/** Register undead with the game state */
	void ValidateUndead();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Undead")
	class UStatAttributeModifier* StatAttributeModifier;
};
