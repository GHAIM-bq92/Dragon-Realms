// Copyright Epic Games, Inc. All Rights Reserved.

#include "DragonRealmsGameModeBase.h"
#include "DragonRealmsPawn.h"

ADragonRealmsGameModeBase::ADragonRealmsGameModeBase()
{
    DefaultPawnClass = ADragonRealmsPawn::StaticClass();
    GameStateClass = nullptr;
    PlayerControllerClass = nullptr;
}
