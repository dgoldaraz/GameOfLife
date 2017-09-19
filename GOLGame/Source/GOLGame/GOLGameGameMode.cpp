// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "GOLGame.h"
#include "GOLGameGameMode.h"
#include "GOLGamePlayerController.h"

AGOLGameGameMode::AGOLGameGameMode()
{
	// no pawn by default
	DefaultPawnClass = NULL;
	// use our own player controller class
	PlayerControllerClass = AGOLGamePlayerController::StaticClass();
}
