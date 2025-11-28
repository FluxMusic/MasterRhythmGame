// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/WorldMapHUD.h"

AWorldMapHUD::AWorldMapHUD()
{
}

void AWorldMapHUD::BeginPlay()
{
	Super::BeginPlay();

	if (PauseMenuWidgetClass)
	{
		PauseMenuInstance = CreateWidget<UPauseMenuWidget>(GetWorld(), PauseMenuWidgetClass);

		if (PauseMenuInstance != nullptr)
		{
			PauseMenuInstance->AddToViewport();
			PauseMenuInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}