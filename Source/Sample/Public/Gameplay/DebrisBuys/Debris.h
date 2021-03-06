// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PurchasableInterface.h"
#include "Debris.generated.h"

UCLASS()
class SAMPLE_API ADebris : public AActor, public IPurchasableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADebris();

	/** Purchases passage to other areas of the map */
	UFUNCTION(BlueprintCallable)
	void Purchase(class ASoldier* Player) override;

	/** Returns the channel that corresponds with the map debris buy */
	FORCEINLINE int32 GetDebrisChannel() const { return DebrisChannel; }

protected:

	/** Register debris with the game state */
	void ValidateDebris();

	/** Removes the debris from the map */
	UFUNCTION(BlueprintCallable)
	void RemoveDebris(class ASoldier* Player);

	UFUNCTION(BlueprintImplementableEvent)
	void OnRemoveDebris(class ASoldier* Player);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDebrisRemoved();

protected:

	/** Debris base */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debris")
	class USceneComponent* DebrisBase;

	/** Debris mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debris")
	class UStaticMeshComponent* DebrisMesh;

	/** Debris trigger */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debris")
	class UBoxComponent* PurchaseTrigger;

	/** Debris name */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Debris")
	FText DebrisName;

	/** Debris cost */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Debris")
	int32 DebrisCost;

	/** Debris channel */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Debris")
	int32 DebrisChannel = 0;
};
