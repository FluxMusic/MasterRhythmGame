// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NewGame->OnClicked.AddDynamic(this,  &UMainMenuWidget::NewGameButtonClicked);
	LoadGame->OnClicked.AddDynamic(this, &UMainMenuWidget::LoadGameButtonClicked);
	Setting->OnClicked.AddDynamic(this,  &UMainMenuWidget::SettingButtonClicked);
	Credits->OnClicked.AddDynamic(this,  &UMainMenuWidget::CreditsButtonClicked);
	Escape->OnClicked.AddDynamic(this,   &UMainMenuWidget::EscapeButtonClicked);

	NewGame->OnHovered.AddDynamic(this, &UMainMenuWidget::NewGameHovered);
	NewGame->OnUnhovered.AddDynamic(this, &UMainMenuWidget::NewGameUnhovered);

	LoadGame->OnHovered.AddDynamic(this, &UMainMenuWidget::LoadGameHovered);
	LoadGame->OnUnhovered.AddDynamic(this, &UMainMenuWidget::LoadGameUnhovered);

	Setting->OnHovered.AddDynamic(this, &UMainMenuWidget::SettingHovered);
	Setting->OnUnhovered.AddDynamic(this, &UMainMenuWidget::SettingUnhovered);

	Credits->OnHovered.AddDynamic(this, &UMainMenuWidget::CreditHovered);
	Credits->OnUnhovered.AddDynamic(this, &UMainMenuWidget::CreditUnhovered);

	Escape->OnHovered.AddDynamic(this, &UMainMenuWidget::EscapeHovered);
	Escape->OnUnhovered.AddDynamic(this, &UMainMenuWidget::EscapeUnhovered);
}

void UMainMenuWidget::NewGameButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UMainMenuWidget::LoadGameButtonClicked()
{
	UKismetSystemLibrary::PrintString(this, FString(TEXT("TODO")), true, true,FLinearColor::Blue, 10.0);
}

void UMainMenuWidget::CreditsButtonClicked()
{
	UWidget::SetVisibility(ESlateVisibility::Hidden);
	//TODO: Set Visibility for Credits Widget
}

void UMainMenuWidget::SettingButtonClicked()
{
	UWidget::SetVisibility(ESlateVisibility::Hidden);
	//TODO: Set Visibility for Setting Widget
}

void UMainMenuWidget::EscapeButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UMainMenuWidget::NewGameHovered()
{
	NewGame->SetBackgroundColor(FLinearColor::Green);

	LoadGame->SetBackgroundColor(FLinearColor::White);
	Credits->SetBackgroundColor(FLinearColor::White);
	Setting->SetBackgroundColor(FLinearColor::White);
	Escape->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuWidget::NewGameUnhovered()
{
	NewGame->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuWidget::LoadGameHovered()
{
	LoadGame->SetBackgroundColor(FLinearColor::Green);

	NewGame->SetBackgroundColor(FLinearColor::White);
	Credits->SetBackgroundColor(FLinearColor::White);
	Setting->SetBackgroundColor(FLinearColor::White);
	Escape->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuWidget::LoadGameUnhovered()
{
	LoadGame->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuWidget::CreditHovered()
{
	Credits->SetBackgroundColor(FLinearColor::Green);

	NewGame->SetBackgroundColor(FLinearColor::White);
	LoadGame->SetBackgroundColor(FLinearColor::White);
	Setting->SetBackgroundColor(FLinearColor::White);
	Escape->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuWidget::CreditUnhovered()
{
	Credits->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuWidget::SettingHovered()
{
	Setting->SetBackgroundColor(FLinearColor::Green);

	NewGame->SetBackgroundColor(FLinearColor::White);
	LoadGame->SetBackgroundColor(FLinearColor::White);
	Credits->SetBackgroundColor(FLinearColor::White);
	Escape->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuWidget::SettingUnhovered()
{
	Setting->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuWidget::EscapeHovered()
{
	Escape->SetBackgroundColor(FLinearColor::Green);

	NewGame->SetBackgroundColor(FLinearColor::White);
	LoadGame->SetBackgroundColor(FLinearColor::White);
	Credits->SetBackgroundColor(FLinearColor::White);
	Setting->SetBackgroundColor(FLinearColor::White);
}

void UMainMenuWidget::EscapeUnhovered()
{
	Escape->SetBackgroundColor(FLinearColor::White);
}
