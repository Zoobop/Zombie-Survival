// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Entity.h"
#include "Soldier.generated.h"

/** Event to call when reloading */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGunStartReload);

UCLASS(BlueprintType)
class SAMPLE_API ASoldier : public AEntity
{
	GENERATED_BODY()
	
/** ------------- Functions ------------- **/
public:
	ASoldier();

  	/** Use the selected item -- usage is determined by the item itself */
  	UFUNCTION(BlueprintCallable)
  	void UseItem(class UEquipableItem* Item);

	UFUNCTION(BlueprintImplementableEvent)
	void CheckRayCastActor(AActor* Actor);


////////////////////////////** --------------- Entity Stat System Interface --------------- **//////////////////////////////////

	/** Call for the entity to receive stat attribute modification */
	void ReceiveStatAttributeModification(TArray<class UStatAttributeModifier*> Modifiers) override;

	/** Call for the entity to apply stat attribute modification */
	TArray<class UStatAttributeModifier*> ApplyStatAttributeModification() override;

protected:

////////////////////////////** --------------- Gun Gameplay --------------- **//////////////////////////////////

	/** ----------- Firing ------------ **/

	/** Starts the process of firing -- Input Action */
	UFUNCTION(BlueprintCallable)
	void StartFire();

	/** Stops the process of firing -- Input Action */
	UFUNCTION(BlueprintCallable)
	void StopFire();

	/** Resets the bCanFire boolean */
	UFUNCTION(BlueprintCallable)
	void ResetFire() { bCanFire = true; }

	/** ----------- ADS ------------ **/

	/** Starts the process of ADSing -- Input Action */
	UFUNCTION(Server, Reliable)
	void ADS();

	/** ADS Start blueprint functionality */
	UFUNCTION(BlueprintImplementableEvent)
	void OnStartADS();

	/** Stops the process of ADSing -- Input Action */
	UFUNCTION(Server, Reliable)
	void StopADS();

	/** ADS Stop blueprint functionality */
	UFUNCTION(BlueprintImplementableEvent)
	void OnStopADS();

	/** ----------- Reloading ------------ **/

	/** Reload the current weapon -- Input Action */
	UFUNCTION(BlueprintCallable)
	void ReloadCurrentGun();

	/** ----------- General Usage ------------ **/

	/** Use the current weapon -- functionality is determined by the weapon itself */
	void UseCurrentGun();


////////////////////////////** --------------- Weapon Swap --------------- **//////////////////////////////////


	/** Switches to the player's next gun -- Input Action */
	void NextWeapon();

	/** Switches to the player's next gun -- Input Action */
	void PreviousWeapon();


////////////////////////////** --------------- Movement --------------- **//////////////////////////////////


	/** Allows the player to start sprinting -- Input Action */
	void StartSprint();

	/** Sprint Start blueprint functionality */
	UFUNCTION(BlueprintImplementableEvent)
	void OnStartSprint();

	/** Stops the player from sprinting -- Input Action */
	void StopSprint();

	/** Sprint Stop blueprint functionality */
	UFUNCTION(BlueprintImplementableEvent)
	void OnStopSprint();

	/** Allows the player to toggle crouching on and off -- Input Action */
	void ToggleCrouch();

	UFUNCTION(Server, Reliable)
	void StartCrouch();

	UFUNCTION(Server, Reliable)
	void StopCrouch();

	/** Crouch Start blueprint functionality */
	UFUNCTION(BlueprintImplementableEvent)
	void OnStartCrouching();

	/** Crouch Stop blueprint functionality */
	UFUNCTION(BlueprintImplementableEvent)
	void OnStopCrouching();

////////////////////////////** --------------- Function Overrides --------------- **//////////////////////////////////


	/** Override the player input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Network replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


/** --------------- Fields --------------- **/

protected:

	/** --------------- Movement --------------- **/


	/** Crouch speed amount that the character gets added on to the character's current movement speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float CrouchSpeedAmount;

	/** Sprint speed amount that the character gets added on to the character's current movement speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SprintSpeedAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bCanSprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", Replicated)
	bool bIsCrouching;


	/** --------------- Gameplay --------------- **/
	

	UPROPERTY(BlueprintReadOnly)
	bool bCanFire;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bIsAiming;

	UPROPERTY(BlueprintReadOnly)
	bool bCanReload;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bIsReloading;


	/** --------------- Components --------------- **/


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USceneComponent* WeaponHolder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UEquipmentComponent* EquipmentComponent;


	/** --------------- Events --------------- **/


	UPROPERTY(BlueprintAssignable)
	FOnGunStartReload OnGunStartReload;


private:

	/** Timer that handles the re-firing of a gun */
	FTimerHandle TimeHandle_HandleRefire;
};
