// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/ESSDeathHandlerInterface.h"
#include "Interfaces/ESSModifierReceptionInterface.h"
#include "EntityStatComponent.generated.h"


UENUM(BlueprintType)
enum struct EFaction : uint8
{
	FACTION_NONE		UMETA(DisplayName = "None"),
	FACTION_SURVIVORS	UMETA(DisplayName = "Survivors"),
	FACTION_UNDEAD		UMETA(DisplayName = "Undead")
};

/** Blueprints will bind to this event to update the UI */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatAttributeChanged);
/** Blueprints will bind to this event to debug stats */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatAttributeChangedDebug, class UStatAttribute*, StatAttribute);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SAMPLE_API UEntityStatComponent : public UActorComponent, public IESSDeathHandlerInterface, public IESSModifierReceptionInterface
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

	/** Call when receiving damage from another entity */
	UFUNCTION(BlueprintCallable)
	void ReceiveStatAttributeModification(TArray<class UStatAttributeModifier*> Modifiers) override;

	/** Set your killer */
	UFUNCTION(BlueprintCallable)
	void SetDamageDealer(class AEntity* Attacker);

	/** Called when Killer changes */
	UFUNCTION()
	virtual void OnRep_Killer();

	/** Handles the entities death */
	void HandleDeath() override;

	/** Checks the associated stat attribute if dead */
	bool CheckForDeath(class UStatAttribute* AssociatedStatAttribute) override;

	/** Replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Returns the faction of the entity */
	FORCEINLINE EFaction GetFaction() const { return Faction; }
	/** Returns the faction of the entity */
	FORCEINLINE class UStatAttributeSet* GetStatAttributeSet() const { return StatAttributeSet; }

protected:

	virtual void BeginPlay() override;

	/** Apply current modifiers */
	UFUNCTION(BlueprintCallable)
	void ApplyCurrentModifiers();

	/** Replaces the necessary modifiers */
	void CheckModifiers(TArray<class UStatAttributeModifier*> Modifiers);

	void Reception();

	/** Clears and/or maintains all modifiers in the list */
	virtual void ModifierMaintenance();

	/** Finds the first stat attribute that is associated with the specified tag */
	UFUNCTION(BlueprintCallable)
	class UStatAttribute* FindStatAttribute(FName Tag) const;

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

	/** Entity who kills you */
	UPROPERTY(ReplicatedUsing = "OnRep_Killer")
	AEntity* Killer;

	UPROPERTY(BlueprintReadWrite, Replicated)
	AEntity* DamageDealer;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnStatAttributeChanged OnStatAttributeChanged;

	UPROPERTY(BlueprintAssignable)
	FOnStatAttributeChangedDebug OnStatAttributeChangedDebug;

	UPROPERTY(BlueprintAssignable)
	FOnHandleDeath OnHandleDeath;

private:

	TArray<class UStatAttributeModifier*> NewModifiers;
};

