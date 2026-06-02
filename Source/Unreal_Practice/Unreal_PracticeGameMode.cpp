// Copyright Epic Games, Inc. All Rights Reserved.

#include "Unreal_PracticeGameMode.h"
#include "Unreal_PracticeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnreal_PracticeGameMode::AUnreal_PracticeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
