// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "GOLGame.h"
#include "GOLGamePlayerController.h"

AGOLGamePlayerController::AGOLGamePlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}
