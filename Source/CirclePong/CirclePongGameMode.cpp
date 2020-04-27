// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CirclePongGameMode.h"
#include "CPControl.h"

ACirclePongGameMode::ACirclePongGameMode()
{
	// Set default pawn class to our character
	//DefaultPawnClass = ACirclePongCharacter::StaticClass();	
	DefaultPawnClass = ACPControl::StaticClass();
	/*HUDClass = nullptr;
	//PlayerControllerClass = ACPControl::StaticClass();
	GameStateClass = nullptr;
	PlayerStateClass = nullptr;
	SpectatorClass = nullptr;*/
}
