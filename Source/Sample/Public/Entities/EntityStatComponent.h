// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EntityStatComponent.generated.h"


UENUM(BlueprintType)
enum struct EFaction : uint8
{
	FACTION_NONE		UMETA(DisplayName = "None"),
	FACTION_SURVIVORS	UMETA(DisplayName = "Survivors"),
	FACTION_UNDEAD		UMETA(DisplayName = "Undead")
};

/** Blueprints will bind to this event to update the UI */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatAttributeChanged, class UStatAttribute*, StatAttribute);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SAMPLE_API UEntityStatComponent : public UActorComponent
{
	GENERATED_BODY()

/** ------------- Functions ------------- **/
public:
	/** Sets default values */
	UEntityStatComponent();

	/** Set Soldier default values */
	void SoldierDefaults();

	/** Set Undead default values */
	void UndeadDefaults();

	/** Applies the Stat Modification Modifier values to the component */
	void ApplyStatAttributeModifiers(TArray<class UStatAttributeModifier*> Modifiers);

	/** Allows blueprints to spawn in an stat attribute set on the entity */
	UFUNCTION(BlueprintCallable)
	void ApplyStatAttributeSet(class UStatAttributeSet* AttributeSet);

	/** Returns the faction of the entity */
	FORCEINLINE EFaction GetFaction() const { return Faction; }

protected:

	virtual void BeginPlay() override;

private:

	/** Pre-set Stat Attribute owners */
	void PresetStatAttributes();

/** --------------- Fields --------------- **/
protected:

	/** Entity Name */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GeneralInfo")
	FText Name;

	/** Entity Faction */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "GeneralInfo")
	EFaction Faction;

	/** Entity Stats */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "GeneralInfo")
	class UStatAttributeSet* StatAttributeSet;

	UPROPERTY(BlueprintAssignable)
	FOnStatAttributeChanged OnStatAttributeChanged;
};

