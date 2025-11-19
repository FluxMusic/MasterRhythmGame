// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/CreditsWidget.h"
#include "Components/Button.h"
#include "Controller/MainMenuController.h"

void UCreditsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());

	if (PlayerController != nullptr)
	{
		MainMenuHud = Cast<AMainMenuHUD>(PlayerController->GetHUD());
	}

	ReturnMainMenu->OnClicked.AddDynamic(this, &UCreditsWidget::ReturnMainMenuButtonClicked);
	ReturnMainMenu->OnHovered.AddDynamic(this, &UCreditsWidget::MainMenuOnHovered);
	ReturnMainMenu->OnUnhovered.AddDynamic(this, &UCreditsWidget::MainMenuOnUnhovered);
}

void UCreditsWidget::ReturnMainMenuButtonClicked()
{
	UWidget::SetVisibility(ESlateVisibility::Hidden);
	if (MainMenuHud != nullptr)
	{
		MainMenuHud->MainMenuInstance->SetVisibility(ESlateVisibility::Visible);
		AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());
		if (PlayerController != nullptr)
		{
			PlayerController->SetControllerState(EControllerState::MainMenu);
		}
	}
}

void UCreditsWidget::MainMenuOnHovered()
{
	ReturnMainMenu->SetBackgroundColor(FLinearColor::Green);
}

void UCreditsWidget::MainMenuOnUnhovered()
{
	ReturnMainMenu->SetBackgroundColor(FLinearColor::White);
}
