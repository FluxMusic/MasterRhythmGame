// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Settings/MainMenuSettingWidget.h"
#include "UI/ButtonWidget.h"
#include "HUD/MainMenuHUD.h"
#include "Controller/MainMenuController.h"
#include "Controller/WorldMapController.h"

void UMainMenuSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());

	if (PlayerController != nullptr)
	{
		MainMenuHud = Cast<AMainMenuHUD>(PlayerController->GetHUD());
	}

	AWorldMapController* WorldMapPlayerController = Cast<AWorldMapController>(GetOwningPlayer());

	if (WorldMapPlayerController != nullptr)
	{
		WorldMapHud = Cast<AWorldMapHUD>(WorldMapPlayerController->GetHUD());
	}

	WBP_AudioSettingWidget->SetVisibility(ESlateVisibility::Hidden);
	WBP_GraphicSettingWidget->SetVisibility(ESlateVisibility::Hidden);

	// Bind button click events only
	Graphic->OnClicked.AddDynamic(this, &UMainMenuSettingWidget::GraphicSettingClicked);
	Audio->OnClicked.AddDynamic(this, &UMainMenuSettingWidget::AudioSettingClicked);
	MainMenu->OnClicked.AddDynamic(this, &UMainMenuSettingWidget::ReturnMainMenuClicked);
}

void UMainMenuSettingWidget::GraphicSettingClicked()
{
	WBP_GraphicSettingWidget->SetVisibility(ESlateVisibility::Visible);
	WBP_AudioSettingWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UMainMenuSettingWidget::AudioSettingClicked()
{
	WBP_GraphicSettingWidget->SetVisibility(ESlateVisibility::Hidden);
	WBP_AudioSettingWidget->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuSettingWidget::ReturnMainMenuClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
	WBP_GraphicSettingWidget->SetVisibility(ESlateVisibility::Hidden);
	WBP_AudioSettingWidget->SetVisibility(ESlateVisibility::Hidden);
	if (MainMenuHud != nullptr)
	{
		MainMenuHud->GetMainMenuInstance()->SetVisibility(ESlateVisibility::Visible);
	}
	if (WorldMapHud != nullptr)
	{
		WorldMapHud->GetPauseMenuInstance()->SetVisibility(ESlateVisibility::Visible);
		AWorldMapController* WorldMapPlayerController = Cast<AWorldMapController>(GetOwningPlayer());
		if (WorldMapPlayerController != nullptr)
		{
			WorldMapPlayerController->SetControllerState(EControllerStateWorldMap::PauseMenu);
		}
	}
}