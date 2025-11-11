// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase/MainMenuGameModeBase.h"
#include "Controller/MainPlayerController.h"

AMainMenuGameModeBase::AMainMenuGameModeBase()
{
	PlayerControllerClass = AMainPlayerController::StaticClass();
}
