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

protected:

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debris")
	FText DebrisName;

	/** Debris cost */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debris")
	int32 DebrisCost;
};
