// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Settings/GraphicSettingWidget.h"
#include "UI/ButtonWidget.h"
#include "HUD/MainMenuHUD.h"
#include "Controller/MainMenuController.h"
#include "Controller/WorldMapController.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetMathLibrary.h"

void UGraphicSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
	if (UserSettings != nullptr)
	{
		UserSettings->LoadSettings();
		WindowMode = UserSettings->GetFullscreenMode();
		GraphicsIndex = UserSettings->GetOverallScalabilityLevel();
		bVSync = UserSettings->IsVSyncEnabled();
		Resolution = UserSettings->GetScreenResolution();
		if (Resolution.X == 1280)
		{
			ResolutionIndex = 0;
		}
		else if (Resolution.X == 1600)
		{
			ResolutionIndex = 1;
		}
		else if (Resolution.X == 1920)
		{
			ResolutionIndex = 2;
		}
		else if (Resolution.X == 2560)
		{
			ResolutionIndex = 3;
		}
		else if (Resolution.X == 3840)
		{
			ResolutionIndex = 4;
		}
	}

	AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());

	if (PlayerController != nullptr)
	{
		MainMenuHud = Cast<AMainMenuHUD>(PlayerController->GetHUD());
	}

	// Bind button click events only
	MainMenu->OnClicked.AddDynamic(this, &UGraphicSettingWidget::ReturnMainMenuClicked);
	WindowModeDown->OnClicked.AddDynamic(this, &UGraphicSettingWidget::WindowModeDownClicked);
	WindowModeUp->OnClicked.AddDynamic(this, &UGraphicSettingWidget::WindowModeUpClicked);
	ResolutionDown->OnClicked.AddDynamic(this, &UGraphicSettingWidget::ResolutionDownClicked);
	ResolutionUp->OnClicked.AddDynamic(this, &UGraphicSettingWidget::ResolutionUpClicked);
	GraphicDown->OnClicked.AddDynamic(this, &UGraphicSettingWidget::GraphicDownClicked);
	GraphicUp->OnClicked.AddDynamic(this, &UGraphicSettingWidget::GraphicUpClicked);
	VSyncDown->OnClicked.AddDynamic(this, &UGraphicSettingWidget::VSyncDownClicked);
	VSyncUp->OnClicked.AddDynamic(this, &UGraphicSettingWidget::VSyncUpClicked);
	Apply->OnClicked.AddDynamic(this, &UGraphicSettingWidget::ApplyClicked);
}

void UGraphicSettingWidget::ReturnMainMenuClicked()
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
}

void UGraphicSettingWidget::WindowModeDownClicked()
{
	int32 ModeAsInt = UKismetMathLibrary::Conv_ByteToInt(WindowMode);
	ModeAsInt--;
	ModeAsInt = FMath::Clamp(ModeAsInt, 0, 2);
	WindowMode = static_cast<EWindowMode::Type>(ModeAsInt);
}

void UGraphicSettingWidget::WindowModeUpClicked()
{
	int32 ModeAsInt = static_cast<int32>(WindowMode);
	ModeAsInt++;
	ModeAsInt = FMath::Clamp(ModeAsInt, 0, 2);
	WindowMode = static_cast<EWindowMode::Type>(ModeAsInt);
}

void UGraphicSettingWidget::ResolutionDownClicked()
{
	ResolutionIndex--;
	ResolutionIndex = FMath::Clamp(ResolutionIndex, 0, 4);

	SwitchResolution(ResolutionIndex);
}

void UGraphicSettingWidget::ResolutionUpClicked()
{
	ResolutionIndex++;
	ResolutionIndex = FMath::Clamp(ResolutionIndex, 0, 4);

	SwitchResolution(ResolutionIndex);
}

void UGraphicSettingWidget::GraphicDownClicked()
{
	GraphicsIndex--;
	GraphicsIndex = FMath::Clamp(GraphicsIndex, 0, 4);
}

void UGraphicSettingWidget::GraphicUpClicked()
{
	GraphicsIndex++;
	GraphicsIndex = FMath::Clamp(GraphicsIndex, 0, 4);
}

void UGraphicSettingWidget::VSyncDownClicked()
{
	bVSync = false;
}

void UGraphicSettingWidget::VSyncUpClicked()
{
	bVSync = true;
}

void UGraphicSettingWidget::ApplyClicked()
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
	if (UserSettings != nullptr)
	{
		UserSettings->SetFullscreenMode(WindowMode);
		UserSettings->SetOverallScalabilityLevel(GraphicsIndex);
		UserSettings->SetVSyncEnabled(bVSync);
		UserSettings->SetScreenResolution(Resolution);
		UserSettings->ApplySettings(true);
	}
}

void UGraphicSettingWidget::SwitchResolution(int32 InResolutionIndex)
{
	switch (InResolutionIndex)
	{
		case 0:
		{
			Resolution = FIntPoint(1280, 720);
			break;
		}
		case 1:
		{
			Resolution = FIntPoint(1600, 900);
			break;
		}
		case 2:
		{
			Resolution = FIntPoint(1920, 1080);
			break;
		}
		case 3:
		{
			Resolution = FIntPoint(2560, 1440);
			break;
		}
		case 4:
		{
			Resolution = FIntPoint(3840, 2160);
			break;
		}
		default:
		{
			// Should not land here
			break;
		}
	}
}
