// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/GameHUD.h"

AGameHUD::AGameHUD()
{
}

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainGameClass)
	{
		MainGameInstance = CreateWidget<UMainGameWidget>(GetWorld(), MainGameClass);
		if (MainGameInstance != nullptr)
		{
			MainGameInstance->AddToViewport();
			MainGameInstance->SetVisibility(ESlateVisibility::Visible);
		}
	}
	if (PauseMenuWidgetClass)
	{
		PauseMenuInstance = CreateWidget<UPauseMenuWidget>(GetWorld(), PauseMenuWidgetClass);

		if (PauseMenuInstance != nullptr)
		{
			PauseMenuInstance->AddToViewport();
			PauseMenuInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
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
