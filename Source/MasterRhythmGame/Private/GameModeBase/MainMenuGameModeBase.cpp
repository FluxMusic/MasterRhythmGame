// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase/MainMenuGameModeBase.h"
#include "Controller/MainMenuController.h"

AMainMenuGameModeBase::AMainMenuGameModeBase()
{
	PlayerControllerClass = AMainMenuController::StaticClass();
}
