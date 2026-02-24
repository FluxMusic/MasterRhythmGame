// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Settings/ControlsWidget.h"
#include "UI/ButtonWidget.h"
#include "Controller/MainMenuController.h"
#include "Controller/WorldMapController.h"
#include "Controller/GameController.h"
#include "GameInstance/MyGameInstance.h"

void UControlsWidget::ReturnMenu()
{
	SetVisibility(ESlateVisibility::Hidden);
	AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->SetControllerState(EControllerState::SettingMenu);
	}

	AWorldMapController* WorldMapController = Cast<AWorldMapController>(GetOwningPlayer());
	if (WorldMapController != nullptr)
	{
		WorldMapController->SetControllerState(EControllerStateWorldMap::SettingMenu);
	}

	AGameController* GameController = Cast<AGameController>(GetOwningPlayer());
	if (GameController != nullptr)
	{
		GameController->SetControllerState(EControllerStateGame::SettingMenu);
	}
}

void UControlsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MainMenu->OnClicked.AddDynamic(this, &UControlsWidget::ReturnMenu);
}