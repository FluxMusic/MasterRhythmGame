// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/LoadMenuWidget.h"
#include "Components/Button.h"
#include "Controller/MainMenuController.h"
#include "Kismet/KismetSystemLibrary.h"

void ULoadMenuWidget::ReturnMainMenuButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
	if (MainMenuHud != nullptr)
	{
		MainMenuHud->GetMainMenuInstance()->SetVisibility(ESlateVisibility::Visible);
	}
	AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->SetControllerState(EControllerState::MainMenu);
	}
	ReturnMainMenuButtonUnhovered();
}

void ULoadMenuWidget::LoadFirstSaveButtonClicked()
{
	UKismetSystemLibrary::PrintString(this, FString(TEXT("TODO")), true, true, FLinearColor::Blue, 10.0);

	//TODO: 
}

void ULoadMenuWidget::LoadSecondSaveButtonClicked()
{
	UKismetSystemLibrary::PrintString(this, FString(TEXT("TODO")), true, true, FLinearColor::Blue, 10.0);

	//TODO:
}

void ULoadMenuWidget::LoadThirdSaveButtonClicked()
{
	UKismetSystemLibrary::PrintString(this, FString(TEXT("TODO")), true, true, FLinearColor::Blue, 10.0);

	//TODO:
}

void ULoadMenuWidget::ReturnMainMenuButtonHovered()
{
	ReturnMainMenuButton->SetBackgroundColor(FLinearColor::Green);

	LoadFirstButton->SetBackgroundColor(FLinearColor::White);
	LoadSecondButton->SetBackgroundColor(FLinearColor::White);
	LoadThirdButton->SetBackgroundColor(FLinearColor::White);
}

void ULoadMenuWidget::ReturnMainMenuButtonUnhovered()
{
	ReturnMainMenuButton->SetBackgroundColor(FLinearColor::White);
}

void ULoadMenuWidget::LoadFirstSaveButtonHovered()
{
	LoadFirstButton->SetBackgroundColor(FLinearColor::Green);

	LoadSecondButton->SetBackgroundColor(FLinearColor::White);
	LoadThirdButton->SetBackgroundColor(FLinearColor::White);
	ReturnMainMenuButton->SetBackgroundColor(FLinearColor::White);
}

void ULoadMenuWidget::LoadFirstSaveButtonUnhovered()
{
	LoadFirstButton->SetBackgroundColor(FLinearColor::White);
}

void ULoadMenuWidget::LoadSecondSaveButtonHovered()
{
	LoadSecondButton->SetBackgroundColor(FLinearColor::Green);

	LoadFirstButton->SetBackgroundColor(FLinearColor::White);
	LoadThirdButton->SetBackgroundColor(FLinearColor::White);
	ReturnMainMenuButton->SetBackgroundColor(FLinearColor::White);
}

void ULoadMenuWidget::LoadSecondSaveButtonUnhovered()
{
	LoadSecondButton->SetBackgroundColor(FLinearColor::White);
}

void ULoadMenuWidget::LoadThirdSaveButtonHovered()
{
	LoadThirdButton->SetBackgroundColor(FLinearColor::Green);

	LoadFirstButton->SetBackgroundColor(FLinearColor::White);
	LoadSecondButton->SetBackgroundColor(FLinearColor::White);
	ReturnMainMenuButton->SetBackgroundColor(FLinearColor::White);
}

void ULoadMenuWidget::LoadThirdSaveButtonUnhovered()
{
	LoadThirdButton->SetBackgroundColor(FLinearColor::White);
}

void ULoadMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ReturnMainMenuButton->OnClicked.AddDynamic(this, &ULoadMenuWidget::ReturnMainMenuButtonClicked);
	LoadFirstButton->OnClicked.AddDynamic(this, &ULoadMenuWidget::LoadFirstSaveButtonClicked);
	LoadSecondButton->OnClicked.AddDynamic(this, &ULoadMenuWidget::LoadSecondSaveButtonClicked);
	LoadThirdButton->OnClicked.AddDynamic(this, &ULoadMenuWidget::LoadThirdSaveButtonClicked);

	ReturnMainMenuButton->OnHovered.AddDynamic(this, &ULoadMenuWidget::ReturnMainMenuButtonHovered);
	ReturnMainMenuButton->OnUnhovered.AddDynamic(this, &ULoadMenuWidget::ReturnMainMenuButtonUnhovered);
	LoadFirstButton->OnHovered.AddDynamic(this, &ULoadMenuWidget::LoadFirstSaveButtonHovered);
	LoadFirstButton->OnUnhovered.AddDynamic(this, &ULoadMenuWidget::LoadFirstSaveButtonUnhovered);
	LoadSecondButton->OnHovered.AddDynamic(this, &ULoadMenuWidget::LoadSecondSaveButtonHovered);
	LoadSecondButton->OnUnhovered.AddDynamic(this, &ULoadMenuWidget::LoadSecondSaveButtonUnhovered);
	LoadThirdButton->OnHovered.AddDynamic(this, &ULoadMenuWidget::LoadThirdSaveButtonHovered);
	LoadThirdButton->OnUnhovered.AddDynamic(this, &ULoadMenuWidget::LoadThirdSaveButtonUnhovered);

	LoadFirstButton->SetBackgroundColor(FLinearColor::Green);

	AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());

	if (PlayerController != nullptr)
	{
		MainMenuHud = Cast<AMainMenuHUD>(PlayerController->GetHUD());
	}
}
