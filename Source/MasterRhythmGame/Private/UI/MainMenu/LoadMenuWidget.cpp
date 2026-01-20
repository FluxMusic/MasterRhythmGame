// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/LoadMenuWidget.h"
#include "UI/ButtonWidget.h"
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

void ULoadMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());

	if (PlayerController != nullptr)
	{
		MainMenuHud = Cast<AMainMenuHUD>(PlayerController->GetHUD());
	}

	// Bind button click events
	ReturnMainMenuButton->OnClicked.AddDynamic(this, &ULoadMenuWidget::ReturnMainMenuButtonClicked);
	LoadFirstButton->OnClicked.AddDynamic(this, &ULoadMenuWidget::LoadFirstSaveButtonClicked);
	LoadSecondButton->OnClicked.AddDynamic(this, &ULoadMenuWidget::LoadSecondSaveButtonClicked);
	LoadThirdButton->OnClicked.AddDynamic(this, &ULoadMenuWidget::LoadThirdSaveButtonClicked);
}
