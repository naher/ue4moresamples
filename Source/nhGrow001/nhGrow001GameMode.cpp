// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "nhGrow001.h"
#include "nhGrow001GameMode.h"
#include "nhGrow001Pawn.h"

AnhGrow001GameMode::AnhGrow001GameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AnhGrow001Pawn::StaticClass();
}

