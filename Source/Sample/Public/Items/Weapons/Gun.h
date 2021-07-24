// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/Weapon.h"
#include "Gun.generated.h"


/** Blueprints will bind to this event to do additional effects after the gun is fired */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGunFired);

UCLASS(Blueprintable, BlueprintType)
class SAMPLE_API AGun : public AWeapon
{
	GENERATED_BODY()

/** ------------- Functions ------------- **/
public:
	AGun();

	/** Specific action use of a gun */
	virtual void Action(FTransform SpawnTransform, class AEntity* Character) override;

	/** Override function for personalized pre-game preparation */
	virtual void LoadBullets() override;

	/** Set the owner of the gun */
	void SetGunOwner(class AEntity* Owner);

	UFUNCTION()
	void ShootGun();

	/** Fires the gun and returns if the gun needs to reload */
	UFUNCTION()
	void Fire();

	UFUNCTION()
	void SetFire(bool State);

	/** Reloads the current gun and sets the ammo values accordingly */
	UFUNCTION(BlueprintCallable)
	bool Reload();

	/** Decrements the current amount of bullets in clip/magazine and returns if there are bullets remaining */
	UFUNCTION()
	bool PostFireCheck();

	UFUNCTION()
	void StopFiring();


////////////////////////////** --------------- Entity Stat System Interface --------------- **//////////////////////////////////

	/** Call to apply stat attribute modification */
	TArray<class UStatAttributeModifier*> ApplyStatAttributeModification() override;

	/** Allows for obtaining stat attribute modifiers through interface */
	class UStatAttributeModifier* GetStatAttributeModifier() override;

	/** Returns the fire location socket name */
	FORCEINLINE FName GetFireLoactionSocketName() const { return FireLocationSocketName; }
	/** Returns the trigger socket name */
	FORCEINLINE FName GetTriggerGripSocketName() const { return TriggerGripSocketName; }
	/** Returns the barrel socket name */
	FORCEINLINE FName GetBarrelGripSocketName() const { return BarrelGripSocketName; }
	/** Returns the ammo type of the gun */
	FORCEINLINE TSubclassOf<class ABullet> GetAmmoType() const { return AmmoType; }
	/** Returns the bullet velocity of the gun */
	FORCEINLINE float GetBulletVelocity() const { return DefaultBulletVelocity; }
	/** Returns the gun's fire rate -- 60.0f represents the RPM (rounds per minute) */
	FORCEINLINE float GetFireRate() const { return 60.0f / DefaultFireRate; }
	/** Returns the gun's bullet spread value (accuracy) */
	FORCEINLINE float GetAccuracy() const { return DefaultAccuracy; }
	/** Returns the gun's max shot distance before bullets disappear */
	FORCEINLINE float GetMaxShotDistance() const { return DefaultMaxShotDistance; }
	/** Returns if there are bullets in the current magazine */
	FORCEINLINE bool HasBulletsToFire() const { return CurrentMagazineAmmo > 0; }
	/** Returns if the current magazine is full */
	FORCEINLINE bool IsMagazineFull() const { return CurrentMagazineAmmo == DefaultAmmoPerMagazine; }
	/** Returns if the gun has no more ammo at all*/
	FORCEINLINE bool HasNoAmmo() const { return CurrentMagazineAmmo == 0 && CurrentAmmoReserves == 0; }
	/** Returns the fire animation */
	FORCEINLINE float GetReloadTime() const { return DefaultReloadTime; }
	/** Returns the fire animation */
	FORCEINLINE class UAnimMontage* GetFireAnimation() const { return FireAnimation; }
	/** Returns the reload animation */
	FORCEINLINE class UAnimMontage* GetReloadAnimation() const { return ReloadAnimation; }
	/** Returns the equip animation */
	FORCEINLINE class UAnimMontage* GetEquipAnimation() const { return EquipAnimation; }

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Checks if the gun is currently able to shoot */
	void CheckFire();

	/** Calculates the gun's damage based on distance and hit location */
	UFUNCTION(BlueprintCallable)
	TArray<class UStatAttributeModifier*> CalculateHit(FHitResult Hit);

	/** Blueprint function to play fire montage */
	UFUNCTION(BlueprintImplementableEvent)
	void OnGunStartFire(class AEntity* GunOwner);

	/** Blueprint functionality when an entity it hit */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnEntityHit(class AEntity* GunOwner, int32 Damage, FHitResult Hit);

private:


/** --------------- Fields --------------- **/
protected:
	/** --------------- General --------------- **/

	/** Owner of the gun */
	class ASoldier* Player;

	/** Fire location socket */
	UPROPERTY(BlueprintReadOnly)
	FName FireLocationSocketName = "Muzzle";

	/** Trigger grip socket name */
	UPROPERTY(BlueprintReadOnly)
	FName TriggerGripSocketName = "Grip_Trigger";

	/** Barrel grip socket name */
	UPROPERTY(BlueprintReadOnly)
	FName BarrelGripSocketName = "Grip_Barrel";

	/** --------------- Gun Specifics --------------- **/

// 	/** Base weapon Stat Modifier */
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|StatModifiers")
// 	class UStatAttributeModifier* StatAttributeModifier;

	/** Modifier Tag */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|StatModifiers")
	FName Tag = "Health";

	/** Base gun damage -- damage value gets applied to the stat modifier */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 0))
	int32 DefaultDamage = 40;

	/** The gun's shooting type */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General")
	bool bIsAutomatic = true;

	/** Base fire rate */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 1, EditCondition = "bIsAutomatic"))
	float DefaultFireRate = 400.0f;

	/** Base accuracy */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 1))
	float DefaultAccuracy = 4.0f;

	/** Base bullet velocity */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 1))
	float DefaultBulletVelocity = 10000.0f;

	/** Base recoil */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 0))
	int32 DefaultRecoilAmount = 0;

	/** Base ammunition in each clip/magazine */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 0))
	int32 DefaultAmmoPerMagazine = 0;

	/** Base number of clips/magazines that the gun starts with */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 0))
	int32 DefaultNumberOfMagazines = 0;

	/** Base range where after the bullet reaches this distance away from the firing, the damage will be reduce by the damage fall-off amount */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 1))
	float DefaultDamageFallOffRange = 1;

	/** Distance between damage fall-off intervals */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 1))
	float DefaultDistanceBetweenIntervals = 10.0f;

	/** Base damage fall-off amount past the damage fall-off range */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 1))
	int32 DefaultDamageFallOffAmount = 1;

	/** Base max shot distance before bullets disappear */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 1))
	float DefaultMaxShotDistance = 1;

	/** Base play rate time of the reload animation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 0.1f))
	float DefaultReloadTime = 1.0f;

	/** Ammo Type the gun is using */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General")
	TSubclassOf<class ABullet> AmmoType;

	/** Current clip/magazine ammo amount */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 0), Replicated)
	int32 CurrentMagazineAmmo;

	/** Current reserves */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 0), Replicated)
	int32 CurrentAmmoReserves;

	/** Gun fire animation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|Animations")
	class UAnimMontage* FireAnimation;

	/** Gun reload animation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|Animations")
	class UAnimMontage* ReloadAnimation;

	/** Gun equip animation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|Animations")
	class UAnimMontage* EquipAnimation;

	/** Event to update UI and play effects */
	UPROPERTY(BlueprintAssignable)
	FOnGunFired OnGunFired;

private:

	/** Bool responsible for allowing firing */
	bool bCanFire;

	/** Timer that handles the re-firing of a gun */
	FTimerHandle TimeHandle_HandleRefire;

};

