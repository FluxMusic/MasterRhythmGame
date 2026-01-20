// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/MainMenuWidget.h"
#include "UI/ButtonWidget.h"
#include "Components/Button.h"
#include "HUD/MainMenuHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Controller/MainMenuController.h"


void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());

	if (PlayerController != nullptr)
	{
		MainMenuHud = Cast<AMainMenuHUD>(PlayerController->GetHUD());
	}

	// Bind button click events
	NewGame->OnClicked.AddDynamic(this, &UMainMenuWidget::NewGameButtonClicked);
	LoadGame->OnClicked.AddDynamic(this, &UMainMenuWidget::LoadGameButtonClicked);
	Setting->OnClicked.AddDynamic(this, &UMainMenuWidget::SettingButtonClicked);
	Credits->OnClicked.AddDynamic(this, &UMainMenuWidget::CreditsButtonClicked);
	Escape->OnClicked.AddDynamic(this, &UMainMenuWidget::EscapeButtonClicked);
}

void UMainMenuWidget::NewGameButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UMainMenuWidget::LoadGameButtonClicked()
{
	AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());

	SetVisibility(ESlateVisibility::Hidden);
	if (MainMenuHud != nullptr)
	{
		MainMenuHud->GetLoadMenuInstance()->SetVisibility(ESlateVisibility::Visible);
	}
	if (PlayerController != nullptr)
	{
		PlayerController->SetControllerState(EControllerState::LoadMenu);
	}
}

void UMainMenuWidget::CreditsButtonClicked()
{
	AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());

	SetVisibility(ESlateVisibility::Hidden);
	if (MainMenuHud != nullptr)
	{
		MainMenuHud->GetCreditsMenuInstance()->SetVisibility(ESlateVisibility::Visible);
	}
	if (PlayerController != nullptr)
	{
		PlayerController->SetControllerState(EControllerState::CreditsMenu);
	}
}

void UMainMenuWidget::SettingButtonClicked()
{
	AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());
	
	UWidget::SetVisibility(ESlateVisibility::Hidden);
	if (MainMenuHud != nullptr)
	{
		MainMenuHud->GetMainMenuSettingInstance()->SetVisibility(ESlateVisibility::Visible);
	}
	if (PlayerController != nullptr)
	{
		PlayerController->SetControllerState(EControllerState::SettingMenu);
	}
}

void UMainMenuWidget::EscapeButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
