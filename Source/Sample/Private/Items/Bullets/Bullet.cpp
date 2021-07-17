// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Bullets/Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Items/Weapons/Gun.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	/** Set default bullet values */
	BulletSpeed = 3000.0f;

	/** Creates default components */
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>("BulletMovement");

	BulletMovement->InitialSpeed = BulletSpeed;
	BulletMovement->MaxSpeed = BulletSpeed;

	BulletMesh->SetRelativeScale3D(FVector(0.03f, 0.03f, 0.03f));

	/** Sets the bullet mesh as the root component */
	RootComponent = BulletMesh;
}

void ABullet::SetUpBullet(class UGun* Gun)
{
  	/** Gun Validation */
  	if (Gun) {
  		OwningGun = Gun;
  
 		/** Set bullet speed */
  		BulletMovement->InitialSpeed = Gun->GetBulletVelocity();
  		BulletMovement->MaxSpeed = Gun->GetBulletVelocity();
  	}
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

class UStatAttributeModifier* ABullet::GetStatAttributeModifier()
{
	return StatAttributeModifier;
}

