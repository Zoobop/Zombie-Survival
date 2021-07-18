// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/Weapon.h"
#include "Interfaces/ESSModifierHandlerInterface.h"
#include "Gun.generated.h"


UCLASS(Blueprintable, BlueprintType, EditinlineNew)
class SAMPLE_API UGun : public UWeapon, public IESSModifierHandlerInterface
{
	GENERATED_BODY()

/** ------------- Functions ------------- **/
public:
	UGun();

	/** Specific action use of a gun */
	virtual void Action(FTransform SpawnTransform, class AEntity* Character) override;

	/** Override function for personalized pre-game preparation */
	virtual void PreLoad() override;

	/** Reloads the current gun and sets the ammo values accordingly */
	UFUNCTION(BlueprintCallable)
	bool Reload();

	/** Decrements the current amount of bullets in clip/magazine and returns if there are bullets remaining */
	bool PostFireCheck();

	/** Allows for obtaining stat attribute modifiers through interface */
	class UStatAttributeModifier* GetStatAttributeModifier() override;

	/** Returns the ammo type of the gun */
	FORCEINLINE TSubclassOf<class ABullet> GetAmmoType() const { return AmmoType; }
	/** Returns the bullet velocity of the gun */
	FORCEINLINE float GetBulletVelocity() const { return DefaultBulletVelocity; }
	/** Returns the gun's fire rate -- 60.0f represents the RPM (rounds per minute) */
	FORCEINLINE float GetFireRate() const { return 60.0f / DefaultFireRate; }
	/** Returns if there are bullets in the current magazine */
	FORCEINLINE bool HasBulletsToFire() const { return CurrentMagazineAmmo != 0; }

	/** --------------- Fields --------------- **/
protected:

	/** --------------- General --------------- **/

	/** Fire location socket */
	const FName FireLocationSocketName = "Muzzle";

	/** --------------- Gun Specifics --------------- **/


	/** Base weapon Stat Modifier */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|StatModifiers")
	class UStatAttributeModifier* StatAttributeModifier;

	/** Base fire rate */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 1))
	float DefaultFireRate;

	/** Base bullet velocity */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 1))
	float DefaultBulletVelocity;

	/** Base recoil */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 0))
	int32 DefaultRecoilAmount;

	/** Base ammunition in each clip/magazine */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 0))
	int32 DefaultAmmoPerMagazine;

	/** Base number of clips/magazines that the gun starts with */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 0))
	int32 DefaultNumberOfMagazines;

	/** Base range where after the bullet reaches this distance away from the firing, the damage will be reduce by the damage fall-off amount */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 1))
	float DefaultDamageFallOffRange;

	/** Base damage fall-off amount past the damage fall-off range */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 1))
	int32 DefaultDamageFallOffAmount;

	/** Ammo Type the gun is using */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|General")
	TSubclassOf<class ABullet> AmmoType;

	/** Current clip/magazine ammo amount */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 0))
	int32 CurrentMagazineAmmo;

	/** Current reserves */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gun|General", meta = (ClampMin = 0))
	int32 CurrentAmmoReserves;

	/** Gun fire animation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|Animations")
	class UAnimMontage* FireAnimation;

	/** Gun reload animation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun|Animations")
	class UAnimMontage* ReloadAnimation;
};

