// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Mainpj1011GameMode.h"
#include "Mainpj1011Character.h"
#include "UObject/ConstructorHelpers.h"

AMainpj1011GameMode::AMainpj1011GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
