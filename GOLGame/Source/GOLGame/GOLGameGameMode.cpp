// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "GOLGameGameMode.h"
#include "GOLGamePlayerController.h"
#include "GOLGamePawn.h"

AGOLGameGameMode::AGOLGameGameMode()
{
	// no pawn by default
	DefaultPawnClass = AGOLGamePawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AGOLGamePlayerController::StaticClass();
}
