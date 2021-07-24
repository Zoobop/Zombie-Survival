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

  	/** Tries to add an equipable item to the equip slot */
  	bool AddToEquipSlot(class AEquipableItem* ItemToAdd);
  
  	/** Tries to remove an equipable item from the equip slot */
  	bool RemoveFromEquipSlot(class AEquipableItem* ItemToRemove);

  	/** Switch weapon index to the new index */
	void PrepWeaponSwap(int32 ValueChange);

  	/** Returns the current weapon */
  	FORCEINLINE class AGun* GetCurrentWeapon() const { return CurrentWeapon; }

protected:

 	/** Spawns the weapon into the world */
	class AGun* HandleWeaponSpawn(TSubclassOf<class AGun> Gun);

	/** Spawns the melee weapon into the world */
	class AMelee* HandleMeleeSpawn(TSubclassOf<class AMelee> Melee);

	/** Spawns the armor into the world */
	class AArmor* HandleArmorSpawn(TSubclassOf<class AArmor> Armor);

	/** Switch out weapons */
	UFUNCTION(BlueprintCallable)
	void SwitchOutWeapon(class AGun* WeaponToSwitchTo);

	// Called when the game starts
	virtual void BeginPlay() override;

	/** Spawn weapon actors into the world */
	void WeaponSetup();

	/** Spawn armor actors into the world */
	void ArmorSetup();

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
  
	/** List of lethals to give to the player at the start of the game */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TArray<TSubclassOf<class ALethal>> DefaultLethals;

	/** List of tacticals to give to the player at the start of the game */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TArray<TSubclassOf<class ATactical>> DefaultTacticals;

	/** List of consumables to give to the player at the start of the game */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TArray<TSubclassOf<class AConsumable>> DefaultConsumables;

	/** Starting melee weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TSubclassOf<class AMelee> DefaultMelee;

  	/** Currently equipped weapons */
  	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Equipment")
  	TArray<class AGun*> EquippedWeapons;
  
  	/** Currently equipped armor */
  	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Equipment")
  	TMap<uint8, class AArmor*> EquippedArmor;

	/** Currently equipped lethals */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Equipment")
	TArray<class ALethal*> EquippedLethals;
  
	/** Currently equipped tacticals */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Equipment")
	TArray<class ATactical*> EquippedTacticals;

  	/** Current Weapon */
  	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Equipment", Replicated)
    class AGun* CurrentWeapon;

	/** Current Melee */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Equipment", Replicated)
	class AMelee* CurrentMelee;

	/** Event to update UI */
	UPROPERTY(BlueprintAssignable, Category = "Equipment")
	FOnEquipmentChanged OnEquipmentChanged;

	/** Event to update weapon mesh */
	UPROPERTY(BlueprintAssignable, Category = "Equipment")
	FOnWeaponSwitched OnWeaponSwitched;

private:

	/** Current weapon index */
	UPROPERTY(ReplicatedUsing = "OnRep_WeaponChanged")
	int32 WeaponIndex;
};
