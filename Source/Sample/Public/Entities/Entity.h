// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/EntityStatSystemInterface.h"
#include "Entity.generated.h"

UCLASS(Abstract)
class SAMPLE_API AEntity : public ACharacter, public IEntityStatSystemInterface
{
	GENERATED_BODY()

/** ------------- Functions ------------- **/
public:
	// Sets default values for this character's properties
	AEntity();

	/** Allows for safe referencing without Actor casting */
	class UEntityStatComponent* GetEntityStatComponent() override;

	/** Call when receiving damage from another entity */
	void ReceiveStatAttributeModification(TArray<class UStatAttributeModifier*> Modifiers) override;

	/** Call when applying damage to another entity */
	TArray<class UStatAttributeModifier*> ApplyStatAttributeModification() override;

	/** Allow for replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Replicate entity pitch and yaw */
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ReplicatePitchAndYaw();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

/** --------------- Fields --------------- **/
public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

protected:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UEntityStatComponent* EntityStatComponent;

	/** Replicated pitch */
	UPROPERTY(BlueprintReadOnly, Replicated)
	float Pitch;

	/** Replicated yaw */
	UPROPERTY(BlueprintReadOnly, Replicated)
	float Yaw;

	/** Default speed of the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float DefaultMovementSpeed;
};
