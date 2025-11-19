// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainMenuHUD.h"

#include "Components/Button.h"
#include "UI/MainMenu/MainMenuWidget.h"


AMainMenuHUD::AMainMenuHUD()
{
	MainMenuWidgetClass = ConstructorHelpers::FClassFinder<UMainMenuWidget>(*ObjectPath).Class;
}

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{
		MainMenuInstance = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);

		if (MainMenuInstance != nullptr)
		{
			MainMenuInstance->AddToViewport();
		}
	}

}
