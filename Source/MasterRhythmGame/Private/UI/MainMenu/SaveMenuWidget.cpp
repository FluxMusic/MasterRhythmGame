// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/SaveMenuWidget.h"
#include "UI/ButtonWidget.h"
#include "Controller/MainMenuController.h"
#include "Controller/GameController.h"
#include "Controller/WorldMapController.h"
#include "GameInstance/MyGameInstance.h"
#include "HUD/MainMenuHUD.h"
#include "HUD/GameHUD.h"
#include "HUD/WorldMapHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void USaveMenuWidget::ReturnMenuButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
	
	FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if (CurrentLevel == "MAP_MainMenu")
	{
		if (MainMenuHud != nullptr)
		{
			MainMenuHud->GetMainMenuInstance()->SetVisibility(ESlateVisibility::Visible);
		}
		AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());
		if (PlayerController != nullptr)
		{
			PlayerController->SetControllerState(EControllerState::MainMenu);
		}
	}
	else if (CurrentLevel == "MAP_WorldMap")
	{
		if (WorldMapHud != nullptr)
		{
			WorldMapHud->GetPauseMenuInstance()->SetVisibility(ESlateVisibility::Visible);
		}
		AWorldMapController* PlayerController = Cast<AWorldMapController>(GetOwningPlayer());
		if (PlayerController != nullptr)
		{
			PlayerController->SetControllerState(EControllerStateWorldMap::PauseMenu);
		}
	}
	else
	{
		if (GameHUD != nullptr)
		{
			GameHUD->GetPauseMenuInstance()->SetVisibility(ESlateVisibility::Visible);
		}
		AGameController* PlayerController = Cast<AGameController>(GetOwningPlayer());
		if (PlayerController != nullptr)
		{
			PlayerController->SetControllerState(EControllerStateGame::PauseMenu);
		}
	}
}

void USaveMenuWidget::SaveFirstSlotButtonClicked()
{
	SaveToSlot(TEXT("SaveSlot1"));
}

void USaveMenuWidget::SaveSecondSlotButtonClicked()
{
	SaveToSlot(TEXT("SaveSlot2"));
}

void USaveMenuWidget::SaveThirdSlotButtonClicked()
{
	SaveToSlot(TEXT("SaveSlot3"));
}

void USaveMenuWidget::SaveToSlot(const FString& SlotName)
{
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->BeforeSaveGame();
		bool bSuccess = GameInstance->SaveGameToSlot(SlotName);
		
		if (bSuccess)
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Game saved to %s successfully!"), *SlotName), true, true, FLinearColor::Green, 5.0f);
		}
		else
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Failed to save game to %s!"), *SlotName), true, true, FLinearColor::Red, 5.0f);
		}
	}
}

void USaveMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if (CurrentLevel == "MAP_MainMenu")
	{
		AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());
		if (PlayerController != nullptr)
		{
			MainMenuHud = Cast<AMainMenuHUD>(PlayerController->GetHUD());
		}
	}
	else if (CurrentLevel == "MAP_WorldMap")
	{
		AWorldMapController* PlayerController = Cast<AWorldMapController>(GetOwningPlayer());
		if (PlayerController != nullptr)
		{
			WorldMapHud = Cast<AWorldMapHUD>(PlayerController->GetHUD());
		}
	}
	else
	{
		AGameController* PlayerController = Cast<AGameController>(GetOwningPlayer());
		if (PlayerController != nullptr)
		{
			GameHUD = Cast<AGameHUD>(PlayerController->GetHUD());
		}
	}

	// Bind button click events
	ReturnMenuButton->OnClicked.AddDynamic(this, &USaveMenuWidget::ReturnMenuButtonClicked);
	SaveFirstButton->OnClicked.AddDynamic(this, &USaveMenuWidget::SaveFirstSlotButtonClicked);
	SaveSecondButton->OnClicked.AddDynamic(this, &USaveMenuWidget::SaveSecondSlotButtonClicked);
	SaveThirdButton->OnClicked.AddDynamic(this, &USaveMenuWidget::SaveThirdSlotButtonClicked);
}
