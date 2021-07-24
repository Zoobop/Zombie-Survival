// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"


AWeapon::AWeapon()
{
	WeaponBase = CreateDefaultSubobject<USceneComponent>("WeaponBase");
	SetRootComponent(WeaponBase);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SkeletalMeshComponent->SetupAttachment(WeaponBase);
}

TArray<class UStatAttributeModifier*> AWeapon::ApplyStatAttributeModification()
{
	TArray<class UStatAttributeModifier*> Modifiers;
	return Modifiers;
}

class UStatAttributeModifier* AWeapon::GetStatAttributeModifier()
{
	return nullptr;
}
