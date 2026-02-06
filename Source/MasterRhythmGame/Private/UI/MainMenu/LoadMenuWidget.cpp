// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/LoadMenuWidget.h"
#include "UI/ButtonWidget.h"
#include "Controller/MainMenuController.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
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
	LoadSaveSlot(TEXT("SaveSlot1"));
}

void ULoadMenuWidget::LoadSecondSaveButtonClicked()
{
	LoadSaveSlot(TEXT("SaveSlot2"));
}

void ULoadMenuWidget::LoadThirdSaveButtonClicked()
{
	LoadSaveSlot(TEXT("SaveSlot3"));
}

void ULoadMenuWidget::LoadSaveSlot(const FString& SlotName)
{
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		if (GameInstance->DoesSaveGameExist(SlotName))
		{
			bool bSuccess = GameInstance->LoadGameFromSlot(SlotName);
			
			if (bSuccess)
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Loaded %s Successfully!"), *SlotName), true, true, FLinearColor::Green, 5.0f);
				
				// Load to world map after loading save
				UGameplayStatics::OpenLevel(GetWorld(), TEXT("WorldMap"));
			}
			else
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Failed to load %s!"), *SlotName), true, true, FLinearColor::Red, 5.0f);
			}
		}
		else
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("No save found in %s!"), *SlotName), true, true, FLinearColor::Yellow, 5.0f);
		}
	}
}

void ULoadMenuWidget::UpdateSaveSlotVisibility()
{
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		// Check each save slot and update button visibility
		if (LoadFirstButton != nullptr)
		{
			bool bSlot1Exists = GameInstance->DoesSaveGameExist(TEXT("SaveSlot1"));
			LoadFirstButton->SetVisibility(bSlot1Exists ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}

		if (LoadSecondButton != nullptr)
		{
			bool bSlot2Exists = GameInstance->DoesSaveGameExist(TEXT("SaveSlot2"));
			LoadSecondButton->SetVisibility(bSlot2Exists ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}

		if (LoadThirdButton != nullptr)
		{
			bool bSlot3Exists = GameInstance->DoesSaveGameExist(TEXT("SaveSlot3"));
			LoadThirdButton->SetVisibility(bSlot3Exists ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
	}
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

	// Update visibility based on existing save slots
	UpdateSaveSlotVisibility();
}

void ULoadMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void ULoadMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void ULoadMenuWidget::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
	
	// Update save slot visibility whenever the widget becomes visible
	if (InVisibility == ESlateVisibility::Visible || InVisibility == ESlateVisibility::SelfHitTestInvisible)
	{
		UpdateSaveSlotVisibility();
	}
}
