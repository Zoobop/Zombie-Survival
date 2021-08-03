// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Entity.h"
#include "Interfaces/ESSModifierApplicationInterface.h"
#include "Undead.generated.h"

UENUM(BlueprintType)
enum struct EState : uint8
{
	STATE_IDLE				UMETA(DisplayName = "Idle"),
	STATE_SLOW_WALK			UMETA(DisplayName = "Slow Walk"),
	STATE_NORMAL_WALK		UMETA(DisplayName = "Normal Walk"),
	STATE_RUN				UMETA(DisplayName = "Run"),
};

UCLASS(BlueprintType)
class SAMPLE_API AUndead : public AEntity, public IESSModifierApplicationInterface
{
	GENERATED_BODY()
	
public:
	AUndead();

	/** Set the movement state */
	void SetMovementState(EState MovementState);

	virtual void SendDeathData(AEntity* Killed) override;

	/** Call when applying damage to another entity */
	UFUNCTION(BlueprintPure)
	TArray<class UStatAttributeModifier*> ApplyStatAttributeModification() override;

protected:

	virtual void BeginPlay() override;

	/** Register undead with the game state */
	void ValidateUndead();

	/** Starts the search for the closest player */
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void InitializeSearch();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void StopSearch();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void FindPlayer(class ASurvivalMode* SurvivalMode);

	void SearchForClosestPlayer();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Undead")
	class UStatAttributeModifier* StatAttributeModifier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Undead")
	EState State = EState::STATE_IDLE;

private:

	FTimerHandle TimerHandle_SearchTimer;

	float SearchInterval = 0.1f;
};
