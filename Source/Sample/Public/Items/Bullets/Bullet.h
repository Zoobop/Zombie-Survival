// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ESSModifierHandlerInterface.h"
#include "Bullet.generated.h"


UCLASS(Abstract, Blueprintable, BlueprintType)
class SAMPLE_API ABullet : public AActor, public IESSModifierHandlerInterface
{
	GENERATED_BODY()

/** ------------- Functions ------------- **/
public:	
	ABullet();

 	/** Sets the bullet values according to the owning gun */
 	void SetUpBullet(class UGun* Gun);

	/** Allows for obtaining stat attribute modifiers through interface */
	class UStatAttributeModifier* GetStatAttributeModifier() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

/** --------------- Fields --------------- **/
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	class UStaticMeshComponent* BulletMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	class UProjectileMovementComponent* BulletMovement;

 	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Bullet")
 	class UGun* OwningGun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet|StatAttributeModifier")
	class UStatAttributeModifier* StatAttributeModifier;
	
	float BulletSpeed;

};
