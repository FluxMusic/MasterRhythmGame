// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Settings/MainMenuSettingWidget.h"
#include "HUD/MainMenuHUD.h"
#include "Components/Button.h"
#include "Controller/MainMenuController.h"

void UMainMenuSettingWidget::NativeConstruct()
{
	AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());

	if (PlayerController != nullptr)
	{
		MainMenuHud = Cast<AMainMenuHUD>(PlayerController->GetHUD());
	}

	WBP_AudioSettingWidget->SetVisibility(ESlateVisibility::Hidden);
	WBP_GraphicSettingWidget->SetVisibility(ESlateVisibility::Hidden);

	Graphic->SetBackgroundColor(FLinearColor::Green);

	Graphic->OnClicked.AddDynamic(this, &UMainMenuSettingWidget::GraphicSettingClicked);
	Audio->OnClicked.AddDynamic(this, &UMainMenuSettingWidget::AudioSettingClicked);
	MainMenu->OnClicked.AddDynamic(this, &UMainMenuSettingWidget::ReturnMainMenuClicked);

	Graphic->OnHovered.AddDynamic(this, &UMainMenuSettingWidget::GraphicButtonHovered);
	Graphic->OnUnhovered.AddDynamic(this, &UMainMenuSettingWidget::GraphicButtonUnhovered);

	Audio->OnHovered.AddDynamic(this, &UMainMenuSettingWidget::AudioButtonHovered);
	Audio->OnUnhovered.AddDynamic(this, &UMainMenuSettingWidget::AudioButtonUnhovered);

	MainMenu->OnHovered.AddDynamic(this, &UMainMenuSettingWidget::ReturnMainMenuButtonHovered);
	MainMenu->OnUnhovered.AddDynamic(this, &UMainMenuSettingWidget::ReturnMainMenuButtonUnhovered);

	Graphic->SetBackgroundColor(FLinearColor::Green);
}

void UMainMenuSettingWidget::GraphicSettingClicked()
{
	WBP_GraphicSettingWidget->SetVisibility(ESlateVisibility::Visible);
	WBP_AudioSettingWidget->SetVisibility(ESlateVisibility::Hidden);
	WBP_GraphicSettingWidget->GetWindowModeDownButton()->SetBackgroundColor(FLinearColor::Green);
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
}

void UMainMenuSettingWidget::GraphicButtonHovered()
{
	Graphic->SetBackgroundColor(FLinearColor::Green);
	Audio->SetBackgroundColor(FLinearColor::White);
	MainMenu->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuSettingWidget::GraphicButtonUnhovered()
{
	Graphic->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuSettingWidget::AudioButtonHovered()
{
	Audio->SetBackgroundColor(FLinearColor::Green);
	Graphic->SetBackgroundColor(FLinearColor::White);
	MainMenu->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuSettingWidget::AudioButtonUnhovered()
{
	Audio->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuSettingWidget::ReturnMainMenuButtonHovered()
{
	MainMenu->SetBackgroundColor(FLinearColor::Green);
	Graphic->SetBackgroundColor(FLinearColor::White);
	Audio->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuSettingWidget::ReturnMainMenuButtonUnhovered()
{
	MainMenu->SetBackgroundColor(FLinearColor::White);
}


