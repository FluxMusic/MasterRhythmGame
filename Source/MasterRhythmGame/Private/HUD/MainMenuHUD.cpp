// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainMenuHUD.h"
#include "UI/MainMenu/MainMenuWidget.h"
#include "UI/MainMenu/CreditsWidget.h"


AMainMenuHUD::AMainMenuHUD()
{
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
	if (CreditsWidgetClass)
	{
		CreditsMenuInstance = CreateWidget<UCreditsWidget>(GetWorld(), CreditsWidgetClass);
		
		if (CreditsMenuInstance != nullptr)
		{
			CreditsMenuInstance->AddToViewport();
			CreditsMenuInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
