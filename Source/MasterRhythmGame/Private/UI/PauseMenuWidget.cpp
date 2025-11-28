// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UPauseMenuWidget::ResumeButtonClicked()
{
}

void UPauseMenuWidget::SaveGameButtonClicked()
{
}

void UPauseMenuWidget::LoadGameButtonClicked()
{
}

void UPauseMenuWidget::SettingsButtonClicked()
{
}

void UPauseMenuWidget::MainMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UPauseMenuWidget::QuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UPauseMenuWidget::ResumeButtonHovered()
{
	ResumeButton->SetBackgroundColor(FLinearColor::Green);

	SaveGameButton->SetBackgroundColor(FLinearColor::White);
	LoadGameButton->SetBackgroundColor(FLinearColor::White);
	SettingsButton->SetBackgroundColor(FLinearColor::White);
	MainMenuButton->SetBackgroundColor(FLinearColor::White);
	QuitButton->SetBackgroundColor(FLinearColor::White);
}

void UPauseMenuWidget::ResumeButtonUnhovered()
{
	ResumeButton->SetBackgroundColor(FLinearColor::White);
}

void UPauseMenuWidget::SaveGameButtonHovered()
{
	SaveGameButton->SetBackgroundColor(FLinearColor::White);

	ResumeButton->SetBackgroundColor(FLinearColor::White);
	LoadGameButton->SetBackgroundColor(FLinearColor::White);
	SettingsButton->SetBackgroundColor(FLinearColor::White);
	MainMenuButton->SetBackgroundColor(FLinearColor::White);
	QuitButton->SetBackgroundColor(FLinearColor::White);
}

void UPauseMenuWidget::SaveGameButtonUnhovered()
{
	SaveGameButton->SetBackgroundColor(FLinearColor::White);
}

void UPauseMenuWidget::LoadGameButtonHovered()
{
	LoadGameButton->SetBackgroundColor(FLinearColor::Green);

	ResumeButton->SetBackgroundColor(FLinearColor::White);
	SaveGameButton->SetBackgroundColor(FLinearColor::White);
	SettingsButton->SetBackgroundColor(FLinearColor::White);
	MainMenuButton->SetBackgroundColor(FLinearColor::White);
	QuitButton->SetBackgroundColor(FLinearColor::White);
}

void UPauseMenuWidget::LoadGameButtonUnhovered()
{
	LoadGameButton->SetBackgroundColor(FLinearColor::White);
}

void UPauseMenuWidget::SettingsButtonHovered()
{
	SettingsButton->SetBackgroundColor(FLinearColor::Green);

	ResumeButton->SetBackgroundColor(FLinearColor::White);
	SaveGameButton->SetBackgroundColor(FLinearColor::White);
	LoadGameButton->SetBackgroundColor(FLinearColor::White);
	MainMenuButton->SetBackgroundColor(FLinearColor::White);
	QuitButton->SetBackgroundColor(FLinearColor::White);
}

void UPauseMenuWidget::SettingsButtonUnhovered()
{
	SettingsButton->SetBackgroundColor(FLinearColor::White);
}

void UPauseMenuWidget::MainMenuButtonHovered()
{
	MainMenuButton->SetBackgroundColor(FLinearColor::Green);

	ResumeButton->SetBackgroundColor(FLinearColor::White);
	SaveGameButton->SetBackgroundColor(FLinearColor::White);
	LoadGameButton->SetBackgroundColor(FLinearColor::White);
	SettingsButton->SetBackgroundColor(FLinearColor::White);
	QuitButton->SetBackgroundColor(FLinearColor::White);
}

void UPauseMenuWidget::MainMenuButtonUnhovered()
{
	MainMenuButton->SetBackgroundColor(FLinearColor::White);
}

void UPauseMenuWidget::QuitButtonHovered()
{
	QuitButton->SetBackgroundColor(FLinearColor::Green);

	ResumeButton->SetBackgroundColor(FLinearColor::White);
	SaveGameButton->SetBackgroundColor(FLinearColor::White);
	LoadGameButton->SetBackgroundColor(FLinearColor::White);
	SettingsButton->SetBackgroundColor(FLinearColor::White);
	MainMenuButton->SetBackgroundColor(FLinearColor::White);
}

void UPauseMenuWidget::QuitButtonUnhovered()
{
	QuitButton->SetBackgroundColor(FLinearColor::White);
}

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton->SetBackgroundColor(FLinearColor::Green);

	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::ResumeButtonClicked);
	ResumeButton->OnHovered.AddDynamic(this, &UPauseMenuWidget::ResumeButtonHovered);
	ResumeButton->OnUnhovered.AddDynamic(this, &UPauseMenuWidget::ResumeButtonUnhovered);

	LoadGameButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::LoadGameButtonClicked);
	LoadGameButton->OnHovered.AddDynamic(this, &UPauseMenuWidget::LoadGameButtonHovered);
	LoadGameButton->OnUnhovered.AddDynamic(this, &UPauseMenuWidget::LoadGameButtonUnhovered);
	
	SaveGameButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::SaveGameButtonClicked);
	SaveGameButton->OnHovered.AddDynamic(this, &UPauseMenuWidget::SaveGameButtonHovered);
	SaveGameButton->OnUnhovered.AddDynamic(this, &UPauseMenuWidget::SaveGameButtonUnhovered);
	
	SettingsButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::SettingsButtonClicked);
	SettingsButton->OnHovered.AddDynamic(this, &UPauseMenuWidget::SettingsButtonHovered);
	SettingsButton->OnUnhovered.AddDynamic(this, &UPauseMenuWidget::SettingsButtonUnhovered);
	
	MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::MainMenuButtonClicked);
	MainMenuButton->OnHovered.AddDynamic(this, &UPauseMenuWidget::MainMenuButtonHovered);
	MainMenuButton->OnUnhovered.AddDynamic(this, &UPauseMenuWidget::MainMenuButtonUnhovered);
	
	QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::QuitButtonClicked);
	QuitButton->OnHovered.AddDynamic(this, &UPauseMenuWidget::QuitButtonHovered);
	QuitButton->OnUnhovered.AddDynamic(this, &UPauseMenuWidget::QuitButtonUnhovered);
}
