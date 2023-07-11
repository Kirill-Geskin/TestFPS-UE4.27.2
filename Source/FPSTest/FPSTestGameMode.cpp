// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSTestGameMode.h"
#include "FPSTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPSTestGameMode::AFPSTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
