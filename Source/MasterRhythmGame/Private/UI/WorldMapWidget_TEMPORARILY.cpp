// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WorldMapWidget_TEMPORARILY.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UWorldMapWidget_TEMPORARILY::LevelOneButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelOneName);
}

void UWorldMapWidget_TEMPORARILY::LevelOneButtonHovered()
{
	LevelOneButton->SetBackgroundColor(FLinearColor::Green);

	LevelTwoButton->SetBackgroundColor(FLinearColor::White);
	LevelThreeButton->SetBackgroundColor(FLinearColor::White);
	LevelFourButton->SetBackgroundColor(FLinearColor::White);
	LevelFiveButton->SetBackgroundColor(FLinearColor::White);
	LevelSixButton->SetBackgroundColor(FLinearColor::White);
	MainMenuButton->SetBackgroundColor(FLinearColor::White);
}

void UWorldMapWidget_TEMPORARILY::LevelOneButtonUnhovered()
{
	LevelOneButton->SetBackgroundColor(FLinearColor::White);
}

void UWorldMapWidget_TEMPORARILY::LevelTwoButtonClicked()
{
	UKismetSystemLibrary::PrintString(this, FString(TEXT("TODO")), true, true, FLinearColor::Blue, 10.0);
}

void UWorldMapWidget_TEMPORARILY::LevelTwoButtonHovered()
{
	LevelTwoButton->SetBackgroundColor(FLinearColor::Green);

	LevelOneButton->SetBackgroundColor(FLinearColor::White);
	LevelThreeButton->SetBackgroundColor(FLinearColor::White);
	LevelFourButton->SetBackgroundColor(FLinearColor::White);
	LevelFiveButton->SetBackgroundColor(FLinearColor::White);
	LevelSixButton->SetBackgroundColor(FLinearColor::White);
	MainMenuButton->SetBackgroundColor(FLinearColor::White);
}

void UWorldMapWidget_TEMPORARILY::LevelTwoButtonUnhovered()
{
	LevelTwoButton->SetBackgroundColor(FLinearColor::White);
}

void UWorldMapWidget_TEMPORARILY::LevelThreeButtonClicked()
{
	UKismetSystemLibrary::PrintString(this, FString(TEXT("TODO")), true, true, FLinearColor::Blue, 10.0);
}

void UWorldMapWidget_TEMPORARILY::LevelThreeButtonHovered()
{
	LevelThreeButton->SetBackgroundColor(FLinearColor::Green);

	LevelOneButton->SetBackgroundColor(FLinearColor::White);
	LevelTwoButton->SetBackgroundColor(FLinearColor::White);
	LevelFourButton->SetBackgroundColor(FLinearColor::White);
	LevelFiveButton->SetBackgroundColor(FLinearColor::White);
	LevelSixButton->SetBackgroundColor(FLinearColor::White);
	MainMenuButton->SetBackgroundColor(FLinearColor::White);
}

void UWorldMapWidget_TEMPORARILY::LevelThreeButtonUnhovered()
{
	LevelThreeButton->SetBackgroundColor(FLinearColor::White);
}

void UWorldMapWidget_TEMPORARILY::LevelFourButtonClicked()
{
	UKismetSystemLibrary::PrintString(this, FString(TEXT("TODO")), true, true, FLinearColor::Blue, 10.0);
}

void UWorldMapWidget_TEMPORARILY::LevelFourButtonHovered()
{
	LevelFourButton->SetBackgroundColor(FLinearColor::Green);

	LevelOneButton->SetBackgroundColor(FLinearColor::White);
	LevelTwoButton->SetBackgroundColor(FLinearColor::White);
	LevelThreeButton->SetBackgroundColor(FLinearColor::White);
	LevelFiveButton->SetBackgroundColor(FLinearColor::White);
	LevelSixButton->SetBackgroundColor(FLinearColor::White);
	MainMenuButton->SetBackgroundColor(FLinearColor::White);
}

void UWorldMapWidget_TEMPORARILY::LevelFourButtonUnhovered()
{
	LevelFourButton->SetBackgroundColor(FLinearColor::White);
}

void UWorldMapWidget_TEMPORARILY::LevelFiveButtonClicked()
{
	UKismetSystemLibrary::PrintString(this, FString(TEXT("TODO")), true, true, FLinearColor::Blue, 10.0);
}

void UWorldMapWidget_TEMPORARILY::LevelFiveButtonHovered()
{
	LevelFiveButton->SetBackgroundColor(FLinearColor::Green);

	LevelOneButton->SetBackgroundColor(FLinearColor::White);
	LevelTwoButton->SetBackgroundColor(FLinearColor::White);
	LevelThreeButton->SetBackgroundColor(FLinearColor::White);
	LevelFourButton->SetBackgroundColor(FLinearColor::White);
	LevelSixButton->SetBackgroundColor(FLinearColor::White);
	MainMenuButton->SetBackgroundColor(FLinearColor::White);
}

void UWorldMapWidget_TEMPORARILY::LevelFiveButtonUnhovered()
{
	LevelFiveButton->SetBackgroundColor(FLinearColor::White);
}

void UWorldMapWidget_TEMPORARILY::LevelSixButtonClicked()
{
	UKismetSystemLibrary::PrintString(this, FString(TEXT("TODO")), true, true, FLinearColor::Blue, 10.0);
}

void UWorldMapWidget_TEMPORARILY::LevelSixButtonHovered()
{
	LevelSixButton->SetBackgroundColor(FLinearColor::Green);

	LevelOneButton->SetBackgroundColor(FLinearColor::White);
	LevelTwoButton->SetBackgroundColor(FLinearColor::White);
	LevelThreeButton->SetBackgroundColor(FLinearColor::White);
	LevelFourButton->SetBackgroundColor(FLinearColor::White);
	LevelFiveButton->SetBackgroundColor(FLinearColor::White);
	MainMenuButton->SetBackgroundColor(FLinearColor::White);
}

void UWorldMapWidget_TEMPORARILY::LevelSixButtonUnhovered()
{
	LevelSixButton->SetBackgroundColor(FLinearColor::White);
}

void UWorldMapWidget_TEMPORARILY::MainMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UWorldMapWidget_TEMPORARILY::MainMenuButtonHovered()
{
	MainMenuButton->SetBackgroundColor(FLinearColor::Green);

	LevelOneButton->SetBackgroundColor(FLinearColor::White);
	LevelTwoButton->SetBackgroundColor(FLinearColor::White);
	LevelThreeButton->SetBackgroundColor(FLinearColor::White);
	LevelFourButton->SetBackgroundColor(FLinearColor::White);
	LevelFiveButton->SetBackgroundColor(FLinearColor::White);
	LevelSixButton->SetBackgroundColor(FLinearColor::White);
}

void UWorldMapWidget_TEMPORARILY::MainMenuButtonUnhovered()
{
	MainMenuButton->SetBackgroundColor(FLinearColor::White);
}

void UWorldMapWidget_TEMPORARILY::NativeConstruct()
{
	Super::NativeConstruct();

	// Initial state
	if (LevelOneButton)
	{
		LevelOneButton->SetBackgroundColor(FLinearColor::Green);
		LevelOneButton->OnClicked.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::LevelOneButtonClicked);
		LevelOneButton->OnHovered.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::LevelOneButtonHovered);
		LevelOneButton->OnUnhovered.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::LevelOneButtonUnhovered);
	}

	if (LevelTwoButton)
	{
		LevelTwoButton->OnClicked.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::LevelTwoButtonClicked);
		LevelTwoButton->OnHovered.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::LevelTwoButtonHovered);
		LevelTwoButton->OnUnhovered.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::LevelTwoButtonUnhovered);
	}

	if (LevelThreeButton)
	{
		LevelThreeButton->OnClicked.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::LevelThreeButtonClicked);
		LevelThreeButton->OnHovered.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::LevelThreeButtonHovered);
		LevelThreeButton->OnUnhovered.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::LevelThreeButtonUnhovered);
	}

	if (LevelFourButton)
	{
		LevelFourButton->OnClicked.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::LevelFourButtonClicked);
		LevelFourButton->OnHovered.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::LevelFourButtonHovered);
		LevelFourButton->OnUnhovered.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::LevelFourButtonUnhovered);
	}

	if (LevelFiveButton)
	{
		LevelFiveButton->OnClicked.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::LevelFiveButtonClicked);
		LevelFiveButton->OnHovered.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::LevelFiveButtonHovered);
		LevelFiveButton->OnUnhovered.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::LevelFiveButtonUnhovered);
	}

	if (LevelSixButton)
	{
		LevelSixButton->OnClicked.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::LevelSixButtonClicked);
		LevelSixButton->OnHovered.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::LevelSixButtonHovered);
		LevelSixButton->OnUnhovered.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::LevelSixButtonUnhovered);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::MainMenuButtonClicked);
		MainMenuButton->OnHovered.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::MainMenuButtonHovered);
		MainMenuButton->OnUnhovered.AddDynamic(this, &UWorldMapWidget_TEMPORARILY::MainMenuButtonUnhovered);
	}
}
