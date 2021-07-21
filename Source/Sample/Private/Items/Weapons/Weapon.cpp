// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"


AWeapon::AWeapon()
{
	WeaponBase = CreateDefaultSubobject<USceneComponent>("WeaponBase");
	SetRootComponent(WeaponBase);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SkeletalMeshComponent->AttachTo(WeaponBase);
}
