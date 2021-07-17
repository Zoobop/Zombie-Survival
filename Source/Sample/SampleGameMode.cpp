// Copyright Epic Games, Inc. All Rights Reserved.

#include "SampleGameMode.h"
#include "SampleCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASampleGameMode::ASampleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
