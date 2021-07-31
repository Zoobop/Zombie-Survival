// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PurchasableInterface.h"
#include "PerkStation.generated.h"

UCLASS()
class SAMPLE_API APerkStation : public AActor, public IPurchasableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APerkStation();

	/** Purchases the display item and gives it to the player */
	UFUNCTION(BlueprintCallable)
	virtual void Purchase(class ASoldier* Player) override;

protected:

	/** Give the player the perk */
	void AcquirePerk(class ASoldier* Player);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAcquirePerk(class ASoldier* Player);

	/** Blueprint event on perk acquisition */
	UFUNCTION(BlueprintImplementableEvent)
	void OnPerkAcquisition();

	/** Check if player already has perk */
	UFUNCTION(BlueprintPure)
	virtual bool HasPerk(class ASoldier* Player) const;

protected:

	/** Perk station base */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PerkStation")
	class USceneComponent* StationBase;

	/** Perk station mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PerkStation")
	class UStaticMeshComponent* StationMesh;

	/** Perk station trigger */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PerkStation")
	class UBoxComponent* PurchaseTrigger;

	/** Perk station emission */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PerkStation")
	class UPointLightComponent* StationEmission;

	/** Perk station perk name */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PerkStation")
	class UTexture2D* PerkIcon;

	/** Perk station perk name */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PerkStation")
	FText PerkName;

	/** Perk station perk cost */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PerkStation")
	int32 PerkCost;

	/** Stat Modifier to be added to the player */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PerkStation")
	TArray<class UStatAttributeModifier*> PerkModifiers;

};