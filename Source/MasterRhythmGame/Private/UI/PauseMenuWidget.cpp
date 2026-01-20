// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenuWidget.h"
#include "UI/ButtonWidget.h"
#include "Components/Button.h"
#include "Controller/WorldMapController.h"
#include "Controller/GameController.h"
#include "HUD/GameHUD.h"
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
	else if (CurrentLevel == "TestMap")
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
			//GameHUD->Get()->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UPauseMenuWidget::SaveGameButtonClicked()
{
	UKismetSystemLibrary::PrintString(this, FString(TEXT("TODO")), true, true, FLinearColor::Blue, 10.0);
}

void UPauseMenuWidget::LoadGameButtonClicked()
{
	UKismetSystemLibrary::PrintString(this, FString(TEXT("TODO")), true, true, FLinearColor::Blue, 10.0);
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
	else if (CurrentLevel == "TestMap")
	{
		AGameController* PlayerController = Cast<AGameController>(GetOwningPlayer());

		if (PlayerController != nullptr)
		{
			GameHUD = Cast<AGameHUD>(PlayerController->GetHUD());
			//PlayerController->SetControllerState(EControllerStateWorldMap::Game);
		}
		if (GameHUD != nullptr)
		{
			//GameHUD->Get()->SetVisibility(ESlateVisibility::Visible);
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
