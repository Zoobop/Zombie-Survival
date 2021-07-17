// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatAttributes/StatAttributeSet.h"
#include "PlayerStatAttributeSet.generated.h"


UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class SAMPLE_API UPlayerStatAttributeSet : public UStatAttributeSet
{
	GENERATED_BODY()

public:

	UPlayerStatAttributeSet();

private:

	class UStatAttribute* Health;

	class UStatAttribute* Stamina;
};
