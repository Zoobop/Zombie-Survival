// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

/** Max number of weapon slots */
const int32 WeaponCapacity = 2;

/** Max number of armor slots */
const int32 ArmorCapacity = 4;

/** Blueprints will bind to this event to update the UI */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentChanged);

/** Blueprints will bind to this event to update the current weapon mesh */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSwitched, class AGun*, WeaponToSwitchTo);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMPLE_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

/** ------------- Functions ------------- **/
public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

	/** Tries to add a gun to the equipped weapons */
	UFUNCTION(BlueprintCallable)
	bool AddGun(TSubclassOf<class AGun> GunToAdd);

	/** Tries to add equipment to the list */
	UFUNCTION(BlueprintCallable)
	bool AddEquipment(TSubclassOf<class AEquipment> EquipmentToAdd);

  	/** Switch weapon index to the new index */
	void PrepWeaponSwap(int32 ValueChange);

	/** Set melee visibility weapon */
	UFUNCTION(BlueprintCallable)
	void SetMeleeVisibility(bool state);

 	/** Spawns the weapon into the world */
	UFUNCTION(BlueprintCallable)
	class AGun* HandleWeaponSpawn(TSubclassOf<class AGun> Gun);

	/** Spawns the melee weapon into the world */
	UFUNCTION(BlueprintCallable)
	class AMelee* HandleMeleeSpawn(TSubclassOf<class AMelee> Melee);

	/** Spawns the armor into the world */
	UFUNCTION(BlueprintCallable)
	class AArmor* HandleArmorSpawn(TSubclassOf<class AArmor> Armor);

	/** Spawns the lethal into the world */
	UFUNCTION(BlueprintCallable)
	class ALethal* HandleLethalSpawn(TSubclassOf<class ALethal> Lethal);

	/** Spawns the lethal into the world */
	UFUNCTION(BlueprintCallable)
	class ATactical* HandleTacticalSpawn(TSubclassOf<class ALethal> Tactical);

	/** Spawns the lethal into the world */
	UFUNCTION(BlueprintCallable)
	class AConsumable* HandleConsumableSpawn(TSubclassOf<class AConsumable> Consumable);

  	/** Returns the current weapon */
  	FORCEINLINE class AGun* GetCurrentWeapon() const { return CurrentWeapon; }
	/** Returns the current melee */
	FORCEINLINE class AMelee* GetCurrentMelee() const { return CurrentMelee; }
	/** Returns if there are multiple equipped weapons */
	FORCEINLINE bool HasMultipleWeapons() const { return EquippedWeapons.Num() > 1; }
	/** Returns the currently equipped weapons */
	FORCEINLINE TArray<class AGun*> GetEquippedWeapons() const { return EquippedWeapons; }
	/** Returns the currently equipped armor */
	FORCEINLINE TMap<uint8, class AArmor*> GetEquippedArmor() const { return EquippedArmor; }
	/** Returns the currently equipped lethals */
	FORCEINLINE class ALethal* GetEquippedLethal() const { return EquippedLethal; }
	/** Returns the currently equipped tacticals */
	FORCEINLINE class ATactical* GetEquippedTactical() const { return EquippedTactical; }
	/** Returns the currently equipped consumables */
	FORCEINLINE class AConsumable* GetEquippedConsumable() const { return EquippedConsumable; }

protected:

	/** Initializes the starting weapons, armor, and other equipment */
	UFUNCTION()
	void InitializeEquipment();

	/** Switch out weapons */
	UFUNCTION(BlueprintCallable)
	void SwitchOutWeapon(class AGun* WeaponToSwitchTo);

	/** Server switch weapons */
	UFUNCTION(Server, Reliable)
	void ServerSwitchWeapon(class AGun* WeaponToSwitchTo);

	/** Tells the server to add a gun */
	UFUNCTION(Server, Reliable)
	void ServerAddGun(TSubclassOf<class AGun> GunToAdd);

	/** Tells the server to add a gun */
	UFUNCTION(Server, Reliable)
	void ServerAddEquipment(TSubclassOf<class AEquipment> EquipmentToAdd);

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_WeaponIndexChanged();

	UFUNCTION()
	void OnRep_WeaponChanged();

	/** Network replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

/** --------------- Fields --------------- **/
protected:

	class ASoldier* Player;

  	/** List of weapons to give to the player at the start of the game */
  	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equipment")
  	TArray<TSubclassOf<class AGun>> DefaultWeapons;
  
  	/** List of armor to give to the player at the start of the game */
  	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equipment")
  	TMap<uint8, TSubclassOf<class AArmor>> DefaultArmor;
  
	/** Lethal to give to the player at the start of the game */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TSubclassOf<class ALethal> DefaultLethal;

	/** Tactical to give to the player at the start of the game */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TSubclassOf<class ATactical> DefaultTactical;

	/** Consumable to give to the player at the start of the game */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TSubclassOf<class AConsumable> DefaultConsumable;

	/** Starting melee weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TSubclassOf<class AMelee> DefaultMelee;

  	/** Currently equipped weapons */
  	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Equipment")
  	TArray<class AGun*> EquippedWeapons;
  
  	/** Currently equipped armor */
  	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Equipment")
  	TMap<uint8, class AArmor*> EquippedArmor;

	/** Currently equipped lethal */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Equipment", Replicated)
	class ALethal* EquippedLethal;
  
	/** Currently equipped tactical */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Equipment", Replicated)
	class ATactical* EquippedTactical;

	/** Currently equipped consumable */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Equipment", Replicated)
	class AConsumable* EquippedConsumable;

  	/** Current Weapon */
  	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Equipment", ReplicatedUsing = "OnRep_WeaponChanged")
    class AGun* CurrentWeapon;

	/** Current Melee */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Equipment", Replicated)
	class AMelee* CurrentMelee;

	/** Event to update UI */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Equipment")
	FOnEquipmentChanged OnEquipmentChanged;

	/** Event to update weapon mesh */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Equipment")
	FOnWeaponSwitched OnWeaponSwitched;

private:

	/** Current weapon index */
	UPROPERTY(ReplicatedUsing = "OnRep_WeaponIndexChanged")
	int32 WeaponIndex;
};
