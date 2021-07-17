// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

/** Max number of weapon slots */
const int32 WeaponCapacity = 4;

/** Max number of armor slots */
const int32 ArmorCapacity = 6;

/** Blueprints will bind to this event to update the UI */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentChanged);

/** Blueprints will bind to this event to update the current weapon mesh */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponSwitched);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMPLE_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

/** ------------- Functions ------------- **/
public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

  	/** Tries to add an equipable item to the equip slot */
  	bool AddToEquipSlot(class UEquipableItem* ItemToAdd);
  
  	/** Tries to remove an equipable item from the equip slot */
  	bool RemoveFromEquipSlot(class UEquipableItem* ItemToRemove);
 

 	/** Display the current weapon's mesh */
 	UFUNCTION(BlueprintCallable)
 	class USkeletalMesh* DisplayWeaponMesh();
 
  	/** Switch weapon */
  	UFUNCTION(BlueprintCallable)
  	void SwitchWeapon(int32 ValueChange);
  
  	/** Returns the current weapon */
  	FORCEINLINE class UGun* GetCurrentWeapon() const { return CurrentWeapon; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

/** --------------- Fields --------------- **/
protected:

  	/** List of weapons to give to the player at the start of the game */
  	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced, Category = "Equipment")
  	TArray<class UGun*> DefaultWeapons;
  
  	/** List of armor to give to the player at the start of the game */
  	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced, Category = "Equipment")
  	TMap<uint8, class UArmor*> DefaultArmor;
  
  	/** Currently equipped weapons */
  	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Equipment")
  	TArray<class UGun*> EquippedWeapons;
  
  	/** Currently equipped armor */
  	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Equipment")
  	TMap<uint8, class UArmor*> EquippedArmor;
  
  	/** Current Weapon */
  	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
    class UGun* CurrentWeapon;

	/** Event to update UI */
	UPROPERTY(BlueprintAssignable, Category = "Equipment")
	FOnEquipmentChanged OnEquipmentChanged;

	/** Event to update weapon mesh */
	UPROPERTY(BlueprintAssignable, Category = "Equipment")
	FOnWeaponSwitched OnWeaponSwitched;

private:

	/** Owning player */
	class ASoldier* Player;

	/** Current weapon index */
	int32 WeaponIndex;
};
