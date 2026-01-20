// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/CreditsWidget.h"
#include "UI/ButtonWidget.h"
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
}

void UCreditsWidget::ReturnMainMenuButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
	if (MainMenuHud != nullptr)
	{
		MainMenuHud->GetMainMenuInstance()->SetVisibility(ESlateVisibility::Visible);
		AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());
		if (PlayerController != nullptr)
		{
			PlayerController->SetControllerState(EControllerState::MainMenu);
		}
	}
}
