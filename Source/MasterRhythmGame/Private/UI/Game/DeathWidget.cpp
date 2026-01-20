// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Game/DeathWidget.h"
#include "UI/ButtonWidget.h"
#include "Kismet/GameplayStatics.h"

void UDeathWidget::RespawnButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()));
}

void UDeathWidget::MainMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuName);
}

void UDeathWidget::WorldMapButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UDeathWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Respawn->OnClicked.AddDynamic(this, &UDeathWidget::RespawnButtonClicked);
	MainMenu->OnClicked.AddDynamic(this, &UDeathWidget::MainMenuButtonClicked);
	WorldMap->OnClicked.AddDynamic(this, &UDeathWidget::WorldMapButtonClicked);
}
