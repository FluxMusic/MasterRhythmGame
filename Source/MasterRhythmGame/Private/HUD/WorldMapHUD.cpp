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
	//if (WorldMapWidgetClass)
	//{
	//	WorldMapWidgetInstance = CreateWidget<UWorldMapWidget_TEMPORARILY>(GetWorld(), WorldMapWidgetClass);

	//	if (WorldMapWidgetInstance != nullptr)
	//	{
	//		WorldMapWidgetInstance->AddToViewport();
	//	}
	//}
	if (MainMenuSettingWidgetClass)
	{
		MainMenuSettingInstance = CreateWidget<UMainMenuSettingWidget>(GetWorld(), MainMenuSettingWidgetClass);
		if (MainMenuSettingInstance != nullptr)
		{
			MainMenuSettingInstance->AddToViewport();
			MainMenuSettingInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (GraphicSettingClass)
	{
		GraphicSettingInstance = CreateWidget<UGraphicSettingWidget>(GetWorld(), GraphicSettingClass);
		if (GraphicSettingInstance != nullptr)
		{
			GraphicSettingInstance->AddToViewport();
			GraphicSettingInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (AudioSettingClass)
	{
		AudioSettingInstance = CreateWidget<UAudioSettingWidget>(GetWorld(), AudioSettingClass);
		if (AudioSettingInstance != nullptr)
		{
			AudioSettingInstance->AddToViewport();
			AudioSettingInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}