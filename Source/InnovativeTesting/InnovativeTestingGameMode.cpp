// Copyright Epic Games, Inc. All Rights Reserved.

#include "InnovativeTestingGameMode.h"
#include "InnovativeTestingCharacter.h"
#include "UObject/ConstructorHelpers.h"

AInnovativeTestingGameMode::AInnovativeTestingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
