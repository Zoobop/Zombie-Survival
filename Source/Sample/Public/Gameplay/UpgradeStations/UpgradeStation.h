// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PurchasableInterface.h"
#include "UpgradeStation.generated.h"

UCLASS()
class SAMPLE_API AUpgradeStation : public AActor, public IPurchasableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUpgradeStation();

	/** Purchases the display item and gives it to the player */
	void Purchase(class ASoldier* Player) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Event called after purchase */
	UFUNCTION(BlueprintImplementableEvent)
	void OnPostPurchase();

	/** Upgrade the current weapon */
	virtual void Upgrade(class ASoldier* Player) PURE_VIRTUAL(AUpgradeStation, );

	/** Event handling upgrade */
	UFUNCTION(BlueprintImplementableEvent)
	void OnUpgrade(class ASoldier* Player);

	/** Checks if the item has been upgrade already */
	UFUNCTION(BlueprintPure)
	virtual bool HasUpgrade(class ASoldier* Player) const;

protected:

	/** Upgrade station mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UpgradeStation")
	class UStaticMeshComponent* StationMesh;

	/** Upgrade station trigger */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UpgradeStation")
	class UBoxComponent* PurchaseTrigger;

	/** Upgrade station emission */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UpgradeStation")
	class UPointLightComponent* StationEmission;

	/** Stat Modifier to be added to the upgraded item */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "UpgradeStation")
	TArray<class UStatAttributeModifier*> UpgradeModifiers;

	/** Upgrade station upgrade cost */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UpgradeStation")
	int32 UpgradeCost;
};
