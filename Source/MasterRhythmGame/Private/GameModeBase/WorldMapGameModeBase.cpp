// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase/WorldMapGameModeBase.h"
#include "HUD/WorldMapHUD.h"
#include "Character/WorldMapPlayerCharacter.h"

AWorldMapGameModeBase::AWorldMapGameModeBase()
{
}

void AWorldMapGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerClass = AWorldMapPlayerCharacter::StaticClass();

	if (WorldMapHudClass)
	{
		HUDClass = WorldMapHudClass;
	}
}
