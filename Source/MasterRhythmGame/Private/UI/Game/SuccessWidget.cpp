// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Game/SuccessWidget.h"
#include "UI/ButtonWidget.h"
#include "Kismet/GameplayStatics.h"

void USuccessWidget::RetryButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()));
}

void USuccessWidget::MainMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuName);
}

void USuccessWidget::WorldMapButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void USuccessWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Retry->OnClicked.AddDynamic(this, &USuccessWidget::RetryButtonClicked);
	MainMenu->OnClicked.AddDynamic(this, &USuccessWidget::MainMenuButtonClicked);
	WorldMap->OnClicked.AddDynamic(this, &USuccessWidget::WorldMapButtonClicked);
}
