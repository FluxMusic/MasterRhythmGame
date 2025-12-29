// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Game/DeathWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UDeathWidget::RespawnButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()));
}

void UDeathWidget::RespawnButtonHovered()
{
	Respawn->SetBackgroundColor(FLinearColor::Green);

	MainMenu->SetBackgroundColor(FLinearColor::White);
	WorldMap->SetBackgroundColor(FLinearColor::White);
}

void UDeathWidget::RespawnButtonUnhovered()
{
	Respawn->SetBackgroundColor(FLinearColor::White);
}

void UDeathWidget::MainMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuName);
}

void UDeathWidget::MainMenuButtonHovered()
{
	MainMenu->SetBackgroundColor(FLinearColor::Green);

	Respawn->SetBackgroundColor(FLinearColor::White);
	WorldMap->SetBackgroundColor(FLinearColor::White);
}

void UDeathWidget::MainMenuButtonUnhovered()
{
	MainMenu->SetBackgroundColor(FLinearColor::White);
}

void UDeathWidget::WorldMapButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UDeathWidget::WorldMapButtonHovered()
{
	WorldMap->SetBackgroundColor(FLinearColor::Green);

	Respawn->SetBackgroundColor(FLinearColor::White);
	MainMenu->SetBackgroundColor(FLinearColor::White);
}

void UDeathWidget::WorldMapButtonUnhovered()
{
	WorldMap->SetBackgroundColor(FLinearColor::White);
}

void UDeathWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Respawn->OnClicked.AddDynamic(this, &UDeathWidget::RespawnButtonClicked);
	Respawn->OnHovered.AddDynamic(this, &UDeathWidget::RespawnButtonHovered);
	Respawn->OnUnhovered.AddDynamic(this, &UDeathWidget::RespawnButtonUnhovered);

	MainMenu->OnClicked.AddDynamic(this, &UDeathWidget::MainMenuButtonClicked);
	MainMenu->OnHovered.AddDynamic(this, &UDeathWidget::MainMenuButtonHovered);
	MainMenu->OnUnhovered.AddDynamic(this, &UDeathWidget::MainMenuButtonUnhovered);

	WorldMap->OnClicked.AddDynamic(this, &UDeathWidget::WorldMapButtonClicked);
	WorldMap->OnHovered.AddDynamic(this, &UDeathWidget::WorldMapButtonHovered);
	WorldMap->OnUnhovered.AddDynamic(this, &UDeathWidget::WorldMapButtonUnhovered);

	Respawn->SetBackgroundColor(FLinearColor::Green);
}
