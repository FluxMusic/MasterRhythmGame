// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenuWidget.h"
#include "UI/ButtonWidget.h"
#include "Controller/WorldMapController.h"
#include "Controller/GameController.h"
#include "GameInstance/MyGameInstance.h"
#include "HUD/GameHUD.h"
#include "HUD/WorldMapHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UPauseMenuWidget::ResumeButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
	FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if (CurrentLevel == "WorldMap")
	{
		AWorldMapController* PlayerController = Cast<AWorldMapController>(GetOwningPlayer());

		if (PlayerController != nullptr)
		{
			WorldMapHud = Cast<AWorldMapHUD>(PlayerController->GetHUD());
			PlayerController->SetControllerState(EControllerStateWorldMap::WorldMap);
			PlayerController->bShowMouseCursor = false;
			FInputModeGameOnly GameInputMode;
			PlayerController->SetInputMode(GameInputMode);
			UGameplayStatics::SetGamePaused(GetWorld(), false);
		}
	}
	else
	{
		AGameController* PlayerController = Cast<AGameController>(GetOwningPlayer());

		if (PlayerController != nullptr)
		{
			GameHUD = Cast<AGameHUD>(PlayerController->GetHUD());
			PlayerController->SetControllerState(EControllerStateGame::Game);
			PlayerController->bShowMouseCursor = false;
			FInputModeGameOnly GameInputMode;
			PlayerController->SetInputMode(GameInputMode);
			UGameplayStatics::SetGamePaused(GetWorld(), false);
		}
		if (GameHUD != nullptr)
		{
			GameHUD->GetMainGameInstance()->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UPauseMenuWidget::SaveGameButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
	FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if (CurrentLevel == "WorldMap")
	{
		AWorldMapController* PlayerController = Cast<AWorldMapController>(GetOwningPlayer());
		if (PlayerController != nullptr)
		{
			WorldMapHud = Cast<AWorldMapHUD>(PlayerController->GetHUD());
			PlayerController->SetControllerState(EControllerStateWorldMap::SaveMenu);
		}
		if (WorldMapHud != nullptr && WorldMapHud->GetSaveMenuInstance() != nullptr)
		{
			WorldMapHud->GetSaveMenuInstance()->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		AGameController* PlayerController = Cast<AGameController>(GetOwningPlayer());
		if (PlayerController != nullptr)
		{
			GameHUD = Cast<AGameHUD>(PlayerController->GetHUD());
			PlayerController->SetControllerState(EControllerStateGame::SaveMenu);
		}
		if (GameHUD != nullptr && GameHUD->GetSaveMenuInstance() != nullptr)
		{
			GameHUD->GetSaveMenuInstance()->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UPauseMenuWidget::LoadGameButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
	FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if (CurrentLevel == "WorldMap")
	{
		AWorldMapController* PlayerController = Cast<AWorldMapController>(GetOwningPlayer());
		if (PlayerController != nullptr)
		{
			WorldMapHud = Cast<AWorldMapHUD>(PlayerController->GetHUD());
			PlayerController->SetControllerState(EControllerStateWorldMap::LoadMenu);
		}
		if (WorldMapHud != nullptr && WorldMapHud->GetLoadMenuInstance() != nullptr)
		{
			WorldMapHud->GetLoadMenuInstance()->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		AGameController* PlayerController = Cast<AGameController>(GetOwningPlayer());
		if (PlayerController != nullptr)
		{
			GameHUD = Cast<AGameHUD>(PlayerController->GetHUD());
			PlayerController->SetControllerState(EControllerStateGame::LoadMenu);
		}
		if (GameHUD != nullptr && GameHUD->GetLoadMenuInstance() != nullptr)
		{
			GameHUD->GetLoadMenuInstance()->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UPauseMenuWidget::SettingsButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
	FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if (CurrentLevel == "WorldMap")
	{
		AWorldMapController* PlayerController = Cast<AWorldMapController>(GetOwningPlayer());

		if (PlayerController != nullptr)
		{
			WorldMapHud = Cast<AWorldMapHUD>(PlayerController->GetHUD());
			PlayerController->SetControllerState(EControllerStateWorldMap::SettingMenu);
		}
		if (WorldMapHud != nullptr)
		{
			WorldMapHud->GetMainMenuSettingInstance()->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		AGameController* PlayerController = Cast<AGameController>(GetOwningPlayer());

		if (PlayerController != nullptr)
		{
			GameHUD = Cast<AGameHUD>(PlayerController->GetHUD());
			PlayerController->SetControllerState(EControllerStateGame::SettingMenu);
		}
		if (GameHUD != nullptr)
		{
			GameHUD->GetMainMenuSettingInstance()->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UPauseMenuWidget::MainMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UPauseMenuWidget::QuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind button click events only
	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::ResumeButtonClicked);
	LoadGameButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::LoadGameButtonClicked);
	SaveGameButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::SaveGameButtonClicked);
	SettingsButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::SettingsButtonClicked);
	MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::MainMenuButtonClicked);
	QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::QuitButtonClicked);
}
