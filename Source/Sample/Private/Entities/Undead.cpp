
// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Undead.h"
#include "Entities/EntityStatComponent.h"
#include "Entities/Soldier.h"
#include "Controllers/EntityController.h"
#include "Gamemodes/SurvivalGameState.h"
#include "Gamemodes/SurvivalMode.h"
#include <DrawDebugHelpers.h>
#include <GameFramework/CharacterMovementComponent.h>

AUndead::AUndead()
{
	GetEntityStatComponent()->UndeadDefaults();
}

void AUndead::SetMovementState(EState MovementState)
{
	State = MovementState;

	switch (State)
	{
	case EState::STATE_SLOW_WALK:
		GetCharacterMovement()->MaxWalkSpeed = 90.0f;
		break;
	case EState::STATE_NORMAL_WALK:
		GetCharacterMovement()->MaxWalkSpeed = 180.0f;
		break;
	case EState::STATE_RUN:
		GetCharacterMovement()->MaxWalkSpeed = 400.0f;
		break;
	default:
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
		break;
	}
}

void AUndead::SendDeathData(AEntity* Killed)
{
	if (HasAuthority()) {
		if (ASoldier* Player = Cast<ASoldier>(Killed)) {
			if (ASurvivalMode* GameMode = Cast<ASurvivalMode>(GetWorld()->GetAuthGameMode())) {
				GameMode->PlayerDied(Player);
			}
		}
	}
}

TArray<class UStatAttributeModifier*> AUndead::ApplyStatAttributeModification()
{
	TArray<class UStatAttributeModifier*> Modifiers;

	Modifiers.Add(StatAttributeModifier);

	return Modifiers;
}

void AUndead::BeginPlay()
{
	Super::BeginPlay();

	ValidateUndead();
}

void AUndead::ValidateUndead()
{
	/** Get the game state */
	if (ASurvivalGameState* GameState = Cast<ASurvivalGameState>(GetWorld()->GetGameState())) {
		GameState->AddSpawnedUndead(this);

		GetEntityStatComponent()->ReConfigureAttributes();
	}
}

void AUndead::AttackPlayer_Implementation()
{
	/** Get Authority */
	if (HasAuthority()) {
		
		/** Find raycast start and end locations */
		FVector TraceStart = GetActorLocation();
		FVector TraceDistance = GetActorLocation() + (GetActorRotation().Vector() * 100.0f);

		/** Setup ray casting */
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceDistance, ECC_Visibility, QueryParams);

		/** Debug line that shows the trace distance */
		DrawDebugLine(GetWorld(), TraceStart, TraceDistance, FColor::Red, false, .1, 0, 2);

		/** Check for hit */
		if (bHit) {

			/** Checks for IEntityStatSystemInterface and IESSModifierReceptionInterface */
			IEntityStatSystemInterface* Interface = Cast<IEntityStatSystemInterface>(HitResult.GetActor());
			if (Interface) {
				/** Apply stat modifiers if not in the same faction */
				if (GetEntityStatComponent()->GetFaction() != Interface->GetEntityStatComponent()->GetFaction()) {
					Interface->GetEntityStatComponent()->SetDamageDealer(this);

					/** Apply the modifiers to the entity */
					Interface->GetEntityStatComponent()->ReceiveStatAttributeModification(ApplyStatAttributeModification());
				}
			}
		}
	}

}

