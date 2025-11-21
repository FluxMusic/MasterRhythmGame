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
			MainMenuInstance->SetVisibility(ESlateVisibility::Visible);
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
	if (MainMenuSettingWidgetClass)
	{
		MainMenuSettingInstance = CreateWidget<UMainMenuSetting>(GetWorld(), MainMenuSettingWidgetClass);
		if (MainMenuSettingInstance != nullptr)
		{
			MainMenuSettingInstance->AddToViewport();
			MainMenuSettingInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (GraphicSettingClass)
	{
		GraphicSettingInstance = CreateWidget<UGraphicSettingWidget>(GetWorld(), GraphicSettingClass);
		if (GraphicSettingInstance != nullptr)
		{
			GraphicSettingInstance->AddToViewport();
			GraphicSettingInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (AudioSettingClass)
	{
		AudioSettingInstance = CreateWidget<UAudioSettingWidget>(GetWorld(), AudioSettingClass);
		if (AudioSettingInstance != nullptr)
		{
			AudioSettingInstance->AddToViewport();
			AudioSettingInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}

}
