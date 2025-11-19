// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase/MainMenuGameModeBase.h"
#include "Controller/MainMenuController.h"
#include "HUD/MainMenuHUD.h"

AMainMenuGameModeBase::AMainMenuGameModeBase()
{
	PlayerControllerClass = AMainMenuController::StaticClass();
	HUDClass = AMainMenuHUD::StaticClass();
}
