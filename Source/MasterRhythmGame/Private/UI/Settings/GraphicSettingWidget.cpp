// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Settings/GraphicSettingWidget.h"
#include "Components/Button.h"
#include "HUD/MainMenuHUD.h"
#include "Controller/MainMenuController.h"
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

	WindowModeDown->SetBackgroundColor(FLinearColor::Green);

	MainMenu->OnClicked.AddDynamic(this, &UGraphicSettingWidget::ReturnMainMenuClicked);
	MainMenu->OnHovered.AddDynamic(this, &UGraphicSettingWidget::ReturnSettingMenuHovered);
	MainMenu->OnUnhovered.AddDynamic(this, &UGraphicSettingWidget::ReturnMainMenuUnhovered);

	WindowModeDown->OnClicked.AddDynamic(this, &UGraphicSettingWidget::WindowModeDownClicked);
	WindowModeDown->OnHovered.AddDynamic(this, &UGraphicSettingWidget::WindowModeDownHovered);
	WindowModeDown->OnUnhovered.AddDynamic(this, &UGraphicSettingWidget::WindowModeDownUnhovered);

	WindowModeUp->OnClicked.AddDynamic(this, &UGraphicSettingWidget::WindowModeUpClicked);
	WindowModeUp->OnHovered.AddDynamic(this, &UGraphicSettingWidget::WindowModeUpHovered);
	WindowModeUp->OnUnhovered.AddDynamic(this, &UGraphicSettingWidget::WindowModeUpUnhovered);

	ResolutionDown->OnClicked.AddDynamic(this, &UGraphicSettingWidget::ResolutionDownClicked);
	ResolutionDown->OnHovered.AddDynamic(this, &UGraphicSettingWidget::ResolutionDownHovered);
	ResolutionDown->OnUnhovered.AddDynamic(this, &UGraphicSettingWidget::ResolutionDownUnhovered);

	ResolutionUp->OnClicked.AddDynamic(this, &UGraphicSettingWidget::ResolutionUpClicked);
	ResolutionUp->OnHovered.AddDynamic(this, &UGraphicSettingWidget::ResolutionUpHovered);
	ResolutionUp->OnUnhovered.AddDynamic(this, &UGraphicSettingWidget::ResolutionUpUnhovered);

	GraphicDown->OnClicked.AddDynamic(this, &UGraphicSettingWidget::GraphicDownClicked);
	GraphicDown->OnHovered.AddDynamic(this, &UGraphicSettingWidget::GraphicDownHovered);
	GraphicDown->OnUnhovered.AddDynamic(this, &UGraphicSettingWidget::GraphicDownUnhovered);

	GraphicUp->OnClicked.AddDynamic(this,&UGraphicSettingWidget::GraphicUpClicked);
	GraphicUp->OnHovered.AddDynamic(this,&UGraphicSettingWidget::GraphicUpHovered);
	GraphicUp->OnUnhovered.AddDynamic(this,&UGraphicSettingWidget::GraphicUpUnhovered);	

	VSyncDown->OnClicked.AddDynamic(this, &UGraphicSettingWidget::VSyncDownClicked);
	VSyncDown->OnHovered.AddDynamic(this, &UGraphicSettingWidget::VSyncDownHovered);
	VSyncDown->OnUnhovered.AddDynamic(this, &UGraphicSettingWidget::VSyncDownUnhovered);

	VSyncUp->OnClicked.AddDynamic(this, &UGraphicSettingWidget::VSyncUpClicked);
	VSyncUp->OnHovered.AddDynamic(this, &UGraphicSettingWidget::VSyncUpHovered);
	VSyncUp->OnUnhovered.AddDynamic(this, &UGraphicSettingWidget::VSyncUpUnhovered);

	Apply->OnClicked.AddDynamic(this, &UGraphicSettingWidget::ApplyClicked);
	Apply->OnHovered.AddDynamic(this, &UGraphicSettingWidget::ApplyHovered);
	Apply->OnUnhovered.AddDynamic(this, &UGraphicSettingWidget::ApplyUnhovered);
}

void UGraphicSettingWidget::ReturnMainMenuClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
	AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());

	if (PlayerController != nullptr)
	{
		PlayerController->SetControllerState(EControllerState::SettingMenu);
	}
	ReturnMainMenuUnhovered();
}

void UGraphicSettingWidget::ReturnSettingMenuHovered()
{
	MainMenu->SetBackgroundColor(FLinearColor::Green);

	WindowModeDown->SetBackgroundColor(FLinearColor::White);
	WindowModeUp->SetBackgroundColor(FLinearColor::White);
	ResolutionDown->SetBackgroundColor(FLinearColor::White);
	ResolutionUp->SetBackgroundColor(FLinearColor::White);
	GraphicDown->SetBackgroundColor(FLinearColor::White);
	GraphicUp->SetBackgroundColor(FLinearColor::White);
	VSyncDown->SetBackgroundColor(FLinearColor::White);
	VSyncUp->SetBackgroundColor(FLinearColor::White);
	Apply->SetBackgroundColor(FLinearColor::White);
}

void UGraphicSettingWidget::ReturnMainMenuUnhovered()
{
	MainMenu->SetBackgroundColor(FLinearColor::White);
}

void UGraphicSettingWidget::WindowModeDownClicked()
{
	int32 ModeAsInt = UKismetMathLibrary::Conv_ByteToInt(WindowMode);
	ModeAsInt--;
	ModeAsInt = FMath::Clamp(ModeAsInt, 0, 2);
	WindowMode = static_cast<EWindowMode::Type>(ModeAsInt);

}

void UGraphicSettingWidget::WindowModeDownHovered()
{
	WindowModeDown->SetBackgroundColor(FLinearColor::Green);

	MainMenu->SetBackgroundColor(FLinearColor::White);
	WindowModeUp->SetBackgroundColor(FLinearColor::White);
	ResolutionDown->SetBackgroundColor(FLinearColor::White);
	ResolutionUp->SetBackgroundColor(FLinearColor::White);
	GraphicDown->SetBackgroundColor(FLinearColor::White);
	GraphicUp->SetBackgroundColor(FLinearColor::White);
	VSyncDown->SetBackgroundColor(FLinearColor::White);
	VSyncUp->SetBackgroundColor(FLinearColor::White);
	Apply->SetBackgroundColor(FLinearColor::White);
}

void UGraphicSettingWidget::WindowModeDownUnhovered()
{
	WindowModeDown->SetBackgroundColor(FLinearColor::White);
}

void UGraphicSettingWidget::WindowModeUpClicked()
{
	int32 ModeAsInt = static_cast<int32>(WindowMode);
	ModeAsInt++;
	ModeAsInt = FMath::Clamp(ModeAsInt, 0, 2);
	WindowMode = static_cast<EWindowMode::Type>(ModeAsInt);
}

void UGraphicSettingWidget::WindowModeUpHovered()
{
	WindowModeUp->SetBackgroundColor(FLinearColor::Green);

	MainMenu->SetBackgroundColor(FLinearColor::White);
	WindowModeDown->SetBackgroundColor(FLinearColor::White);
	ResolutionDown->SetBackgroundColor(FLinearColor::White);
	ResolutionUp->SetBackgroundColor(FLinearColor::White);
	GraphicDown->SetBackgroundColor(FLinearColor::White);
	GraphicUp->SetBackgroundColor(FLinearColor::White);
	VSyncDown->SetBackgroundColor(FLinearColor::White);
	VSyncUp->SetBackgroundColor(FLinearColor::White);
	Apply->SetBackgroundColor(FLinearColor::White);
}

void UGraphicSettingWidget::WindowModeUpUnhovered()
{
	WindowModeUp->SetBackgroundColor(FLinearColor::White);
}

void UGraphicSettingWidget::ResolutionDownClicked()
{
	ResolutionIndex--;
	ResolutionIndex = FMath::Clamp(ResolutionIndex, 0, 4);

	SwitchResolution(ResolutionIndex);
}

void UGraphicSettingWidget::ResolutionDownHovered()
{
	ResolutionDown->SetBackgroundColor(FLinearColor::Green);
	
	MainMenu->SetBackgroundColor(FLinearColor::White);
	WindowModeDown->SetBackgroundColor(FLinearColor::White);
	WindowModeUp->SetBackgroundColor(FLinearColor::White);
	ResolutionUp->SetBackgroundColor(FLinearColor::White);
	GraphicDown->SetBackgroundColor(FLinearColor::White);
	GraphicUp->SetBackgroundColor(FLinearColor::White);
	VSyncDown->SetBackgroundColor(FLinearColor::White);
	VSyncUp->SetBackgroundColor(FLinearColor::White);
	Apply->SetBackgroundColor(FLinearColor::White);
}

void UGraphicSettingWidget::ResolutionDownUnhovered()
{
	ResolutionDown->SetBackgroundColor(FLinearColor::White);
}

void UGraphicSettingWidget::ResolutionUpClicked()
{
	ResolutionIndex++;
	ResolutionIndex = FMath::Clamp(ResolutionIndex, 0, 4);

	SwitchResolution(ResolutionIndex);
}

void UGraphicSettingWidget::ResolutionUpHovered()
{
	ResolutionUp->SetBackgroundColor(FLinearColor::Green);

	MainMenu->SetBackgroundColor(FLinearColor::White);
	WindowModeDown->SetBackgroundColor(FLinearColor::White);
	WindowModeUp->SetBackgroundColor(FLinearColor::White);
	ResolutionDown->SetBackgroundColor(FLinearColor::White);
	GraphicDown->SetBackgroundColor(FLinearColor::White);
	GraphicUp->SetBackgroundColor(FLinearColor::White);
	VSyncDown->SetBackgroundColor(FLinearColor::White);
	VSyncUp->SetBackgroundColor(FLinearColor::White);
	Apply->SetBackgroundColor(FLinearColor::White);
}

void UGraphicSettingWidget::ResolutionUpUnhovered()
{
	ResolutionUp->SetBackgroundColor(FLinearColor::White);
}

void UGraphicSettingWidget::GraphicDownClicked()
{
	GraphicsIndex--;
	GraphicsIndex = FMath::Clamp(GraphicsIndex, 0, 4);
}

void UGraphicSettingWidget::GraphicDownHovered()
{
	GraphicDown->SetBackgroundColor(FLinearColor::Green);

	MainMenu->SetBackgroundColor(FLinearColor::White);
	WindowModeDown->SetBackgroundColor(FLinearColor::White);
	WindowModeUp->SetBackgroundColor(FLinearColor::White);
	ResolutionDown->SetBackgroundColor(FLinearColor::White);
	ResolutionUp->SetBackgroundColor(FLinearColor::White);
	GraphicUp->SetBackgroundColor(FLinearColor::White);
	VSyncDown->SetBackgroundColor(FLinearColor::White);
	VSyncUp->SetBackgroundColor(FLinearColor::White);
	Apply->SetBackgroundColor(FLinearColor::White);
}

void UGraphicSettingWidget::GraphicDownUnhovered()
{
	GraphicDown->SetBackgroundColor(FLinearColor::White);
}

void UGraphicSettingWidget::GraphicUpClicked()
{
	GraphicsIndex++;
	GraphicsIndex = FMath::Clamp(GraphicsIndex, 0, 4);
}

void UGraphicSettingWidget::GraphicUpHovered()
{
	GraphicUp->SetBackgroundColor(FLinearColor::Green);

	MainMenu->SetBackgroundColor(FLinearColor::White);
	WindowModeDown->SetBackgroundColor(FLinearColor::White);
	WindowModeUp->SetBackgroundColor(FLinearColor::White);
	ResolutionDown->SetBackgroundColor(FLinearColor::White);
	ResolutionUp->SetBackgroundColor(FLinearColor::White);
	GraphicDown->SetBackgroundColor(FLinearColor::White);
	VSyncDown->SetBackgroundColor(FLinearColor::White);
	VSyncUp->SetBackgroundColor(FLinearColor::White);
	Apply->SetBackgroundColor(FLinearColor::White);
}

void UGraphicSettingWidget::GraphicUpUnhovered()
{
	GraphicUp->SetBackgroundColor(FLinearColor::White);
}

void UGraphicSettingWidget::VSyncDownClicked()
{
	bVSync = false;
}

void UGraphicSettingWidget::VSyncDownHovered()
{
	VSyncDown->SetBackgroundColor(FLinearColor::Green);

	MainMenu->SetBackgroundColor(FLinearColor::White);
	WindowModeDown->SetBackgroundColor(FLinearColor::White);
	WindowModeUp->SetBackgroundColor(FLinearColor::White);
	ResolutionDown->SetBackgroundColor(FLinearColor::White);
	ResolutionUp->SetBackgroundColor(FLinearColor::White);
	GraphicDown->SetBackgroundColor(FLinearColor::White);
	GraphicUp->SetBackgroundColor(FLinearColor::White);
	VSyncUp->SetBackgroundColor(FLinearColor::White);
	Apply->SetBackgroundColor(FLinearColor::White);
}

void UGraphicSettingWidget::VSyncDownUnhovered()
{
	VSyncDown->SetBackgroundColor(FLinearColor::White);
}

void UGraphicSettingWidget::VSyncUpClicked()
{
	bVSync = true;
}

void UGraphicSettingWidget::VSyncUpHovered()
{
	VSyncUp->SetBackgroundColor(FLinearColor::Green);

	MainMenu->SetBackgroundColor(FLinearColor::White);
	WindowModeDown->SetBackgroundColor(FLinearColor::White);
	WindowModeUp->SetBackgroundColor(FLinearColor::White);
	ResolutionDown->SetBackgroundColor(FLinearColor::White);
	ResolutionUp->SetBackgroundColor(FLinearColor::White);
	GraphicDown->SetBackgroundColor(FLinearColor::White);
	GraphicUp->SetBackgroundColor(FLinearColor::White);
	VSyncDown->SetBackgroundColor(FLinearColor::White);
	Apply->SetBackgroundColor(FLinearColor::White);
}

void UGraphicSettingWidget::VSyncUpUnhovered()
{
	VSyncUp->SetBackgroundColor(FLinearColor::White);
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

void UGraphicSettingWidget::ApplyHovered()
{
	Apply->SetBackgroundColor(FLinearColor::Green);

	MainMenu->SetBackgroundColor(FLinearColor::White);
	WindowModeDown->SetBackgroundColor(FLinearColor::White);
	WindowModeUp->SetBackgroundColor(FLinearColor::White);
	ResolutionDown->SetBackgroundColor(FLinearColor::White);
	ResolutionUp->SetBackgroundColor(FLinearColor::White);
	GraphicDown->SetBackgroundColor(FLinearColor::White);
	GraphicUp->SetBackgroundColor(FLinearColor::White);
	VSyncDown->SetBackgroundColor(FLinearColor::White);
	VSyncUp->SetBackgroundColor(FLinearColor::White);
}

void UGraphicSettingWidget::ApplyUnhovered()
{
	Apply->SetBackgroundColor(FLinearColor::White);
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
