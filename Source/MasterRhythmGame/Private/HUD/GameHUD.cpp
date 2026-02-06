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
	if (SaveMenuWidgetClass)
	{
		SaveMenuInstance = CreateWidget<USaveMenuWidget>(GetWorld(), SaveMenuWidgetClass);
		if (SaveMenuInstance != nullptr)
		{
			SaveMenuInstance->AddToViewport();
			SaveMenuInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (LoadMenuWidgetClass)
	{
		LoadMenuInstance = CreateWidget<ULoadMenuWidget>(GetWorld(), LoadMenuWidgetClass);
		if (LoadMenuInstance != nullptr)
		{
			LoadMenuInstance->AddToViewport();
			LoadMenuInstance->SetVisibility(ESlateVisibility::Hidden);
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
	if (DeathWidgetClass)
	{
		DeathWidgetInstance = CreateWidget<UDeathWidget>(GetWorld(), DeathWidgetClass);
		if (DeathWidgetInstance != nullptr)
		{
			DeathWidgetInstance->AddToViewport();
			DeathWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (SuccessWidgetClass)
	{
		SuccessWidgetInstance = CreateWidget<USuccessWidget>(GetWorld(), SuccessWidgetClass);
		if (SuccessWidgetInstance != nullptr)
		{
			SuccessWidgetInstance->AddToViewport();
			SuccessWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (BlackscreenClass)
	{
		BlackscreenInstance = CreateWidget<UBlackscreenWidget>(GetWorld(), BlackscreenClass);
		if (BlackscreenInstance != nullptr)
		{
			BlackscreenInstance->AddToViewport();
			BlackscreenInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
