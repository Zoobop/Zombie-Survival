// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Entity.h"
#include "Soldier.generated.h"

/** Event to call when reloading */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGunReloaded);

UCLASS(BlueprintType)
class SAMPLE_API ASoldier : public AEntity
{
	GENERATED_BODY()
	
/** ------------- Functions ------------- **/
public:
	ASoldier();

  	/** Use the selected item -- usage is determined by the item itself */
  	UFUNCTION(BlueprintCallable)
  	void UseItem(class AEquipableItem* Item);

	virtual void SendDeathData(AEntity* Killed) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerInitialized();

	/** Starts to reload if magazine is empty */
	void CheckReload();

	FORCEINLINE class USceneComponent* GetWeaponHolder() const { return WeaponHolder; }
	FORCEINLINE class UChildActorComponent* GetWeaponActor() const { return WeaponActor; }
	FORCEINLINE class USceneComponent* GetMeleeHolder() const { return MeleeHolder; }
	FORCEINLINE class UChildActorComponent* GetMeleeActor() const { return MeleeActor; }
	FORCEINLINE class UEquipmentComponent* GetEquipmentComponent() const { return EquipmentComponent; }

protected:

////////////////////////////** --------------- Gun Gameplay --------------- **//////////////////////////////////

	/** ----------- Firing ------------ **/

	UFUNCTION(Server, Reliable)
	void ServerOnFire();

	UFUNCTION(Server, Reliable)
	void ServerStopFire();

	/** Starts the process of firing -- Input Action */
	UFUNCTION(BlueprintCallable)
	void StartFire();

	/** Stops the process of firing -- Input Action */
	UFUNCTION(BlueprintCallable)
	void StopFire();

	/** Use the current weapon -- functionality is determined by the weapon itself */
	void UseCurrentGun();

	/** ----------- Melee ------------ **/

	UFUNCTION(Server, Reliable)
	void ServerOnMelee();

	UFUNCTION(Server, Reliable)
	void ServerStopMelee();

	/** Starts the process of meleeing -- Input Action */
	UFUNCTION(BlueprintCallable)
	void StartMelee();

	UFUNCTION(BlueprintCallable)
	void FinishMelee();

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartMelee();


	/** ----------- ADS ------------ **/

	UFUNCTION(Server, Reliable)
	void ServerADS();

	/** Starts the process of ADSing -- Input Action */
	void ADS();

	/** ADS Start blueprint functionality */
	UFUNCTION(BlueprintImplementableEvent)
	void OnStartADS();

	UFUNCTION(Server, Reliable)
	void ServerStopADS();

	/** Stops the process of ADSing -- Input Action */
	void StopADS();

	/** ADS Stop blueprint functionality */
	UFUNCTION(BlueprintImplementableEvent)
	void OnStopADS();

	/** ----------- Reloading ------------ **/

	/** Server start reload call */
	UFUNCTION(Server, Reliable)
	void ServerStartReload();

	/** Reload the current weapon -- Input Action */
	UFUNCTION(BlueprintCallable)
	void StartReload();

	UFUNCTION(BlueprintImplementableEvent)
	void OnGunStartReload(class AGun* Gun);

	UFUNCTION(BlueprintCallable)
	void FinishReload();

	UFUNCTION(BlueprintCallable)
	void ResetReload(bool State);

	/** ----------- General Usage ------------ **/

	UFUNCTION(Server, Reliable)
	void ServerInteract();

	void Interact();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartInteract();

	UFUNCTION(BlueprintImplementableEvent)
	void OnInteract(FHitResult Hit);

////////////////////////////** --------------- Weapon Swap --------------- **//////////////////////////////////


	UFUNCTION(Server, Reliable)
	void ServerSwitchWeapon(int32 Index);

	/** Switches to the player's next gun -- Input Action */
	void NextWeapon();

	/** Switches to the player's next gun -- Input Action */
	void PreviousWeapon();

	UFUNCTION(BlueprintCallable)
	void ResetWeaponSwap();


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

	/** Called at the start of the game */
	virtual void BeginPlay() override;

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
	

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bCanInteract;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bCanMelee;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bCanReload;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bCanSwitchWeapons;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bIsAiming;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bIsReloading;


	/** --------------- Components --------------- **/


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USceneComponent* WeaponHolder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UChildActorComponent* WeaponActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USceneComponent* MeleeHolder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UChildActorComponent* MeleeActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UEquipmentComponent* EquipmentComponent;


	/** --------------- Events --------------- **/


	/** Event to add more functionality to reloading */
	UPROPERTY(BlueprintAssignable)
	FOnGunReloaded OnGunReloaded;

};
