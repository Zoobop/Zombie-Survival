// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PurchasableInterface.h"
#include "WallBuy.generated.h"

UCLASS()
class SAMPLE_API AWallBuy : public AActor, public IPurchasableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallBuy();

	/** Purchases the display item and gives it to the player */
	void Purchase(class ASoldier* Player) override;

	/** Checks the player points to see if there is enough */
	bool CheckPlayerPoints(class ASoldier* Player, int32 PointsNeeded) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Gives the item to the player */
	UFUNCTION(BlueprintImplementableEvent)
	void OnFirstTimePurchase(class ASoldier* Player);

	/** Gives the more of the item or more ammo, etc... */
	UFUNCTION(BlueprintImplementableEvent)
	void OnRefill(class ASoldier* Player);

	/** Displays the weapon name and cost for purchase */
	UFUNCTION(BlueprintImplementableEvent)
	void OnDisplay();

	/** Checks if the player contains the item */
	UFUNCTION(BlueprintCallable)
	bool CheckItem(class ASoldier* Player);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WallBuy")
	TSubclassOf<class AEquipableItem> ItemToBuy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WallBuy")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WallBuy", meta = (ClampMin = 0))
	int32 PurchasePrice;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WallBuy", meta = (ClampMin = 0))
	int32 RefillPrice;

	UPROPERTY(BlueprintReadOnly)
	bool bHasBeenPurchased;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* PurchaseTrigger;

};
