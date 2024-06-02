// Copyright Epic Games, Inc. All Rights Reserved.

#include "cpp_tutorialGameMode.h"
#include "cpp_tutorialCharacter.h"
#include "UObject/ConstructorHelpers.h"

Acpp_tutorialGameMode::Acpp_tutorialGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
