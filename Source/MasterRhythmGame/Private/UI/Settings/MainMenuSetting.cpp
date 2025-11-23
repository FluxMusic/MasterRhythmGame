// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Settings/MainMenuSetting.h"
#include "HUD/MainMenuHUD.h"
#include "Components/Button.h"
#include "Controller/MainMenuController.h"

void UMainMenuSetting::NativeConstruct()
{
	AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());

	if (PlayerController != nullptr)
	{
		MainMenuHud = Cast<AMainMenuHUD>(PlayerController->GetHUD());
	}

	WBP_AudioSettingWidget->SetVisibility(ESlateVisibility::Hidden);
	WBP_GraphicSettingWidget->SetVisibility(ESlateVisibility::Hidden);

	Graphic->SetBackgroundColor(FLinearColor::Green);

	Graphic->OnClicked.AddDynamic(this, &UMainMenuSetting::GraphicSettingClicked);
	Audio->OnClicked.AddDynamic(this, &UMainMenuSetting::AudioSettingClicked);
	MainMenu->OnClicked.AddDynamic(this, &UMainMenuSetting::ReturnMainMenuClicked);

	Graphic->OnHovered.AddDynamic(this, &UMainMenuSetting::GraphicButtonHovered);
	Graphic->OnUnhovered.AddDynamic(this, &UMainMenuSetting::GraphicButtonUnhovered);

	Audio->OnHovered.AddDynamic(this, &UMainMenuSetting::AudioButtonHovered);
	Audio->OnUnhovered.AddDynamic(this, &UMainMenuSetting::AudioButtonUnhovered);

	MainMenu->OnHovered.AddDynamic(this, &UMainMenuSetting::ReturnMainMenuButtonHovered);
	MainMenu->OnUnhovered.AddDynamic(this, &UMainMenuSetting::ReturnMainMenuButtonUnhovered);

	Graphic->SetBackgroundColor(FLinearColor::Green);
}

void UMainMenuSetting::GraphicSettingClicked()
{
	WBP_GraphicSettingWidget->SetVisibility(ESlateVisibility::Visible);
	WBP_AudioSettingWidget->SetVisibility(ESlateVisibility::Hidden);
	WBP_GraphicSettingWidget->WindowModeDown->SetBackgroundColor(FLinearColor::Green);
}

void UMainMenuSetting::AudioSettingClicked()
{
	WBP_GraphicSettingWidget->SetVisibility(ESlateVisibility::Hidden);
	WBP_AudioSettingWidget->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuSetting::ReturnMainMenuClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
	WBP_GraphicSettingWidget->SetVisibility(ESlateVisibility::Hidden);
	WBP_AudioSettingWidget->SetVisibility(ESlateVisibility::Hidden);
	if (MainMenuHud != nullptr)
	{
		MainMenuHud->MainMenuInstance->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainMenuSetting::GraphicButtonHovered()
{
	Graphic->SetBackgroundColor(FLinearColor::Green);
	Audio->SetBackgroundColor(FLinearColor::White);
	MainMenu->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuSetting::GraphicButtonUnhovered()
{
	Graphic->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuSetting::AudioButtonHovered()
{
	Audio->SetBackgroundColor(FLinearColor::Green);
	Graphic->SetBackgroundColor(FLinearColor::White);
	MainMenu->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuSetting::AudioButtonUnhovered()
{
	Audio->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuSetting::ReturnMainMenuButtonHovered()
{
	MainMenu->SetBackgroundColor(FLinearColor::Green);
	Graphic->SetBackgroundColor(FLinearColor::White);
	Audio->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuSetting::ReturnMainMenuButtonUnhovered()
{
	MainMenu->SetBackgroundColor(FLinearColor::White);
}


